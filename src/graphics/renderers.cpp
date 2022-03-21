
#include "renderers.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <random>


// ------------------------------------
// --- Renderer interface ---
Renderer::Renderer(ShaderProgram &shader_program) : m_shader_prog(shader_program)
{

}

// ------------------------------------
// --- Skybox renderer ---

SkyBoxRenderer::SkyBoxRenderer(ShaderProgram &shader_prog) 
: Renderer(shader_prog), m_cubemap_texture(nullptr)
{
    this->prepare();
}

SkyBoxRenderer::SkyBoxRenderer(ShaderProgram &shader_prog, CubeMapTexture &cubemap_texture)
	: Renderer(shader_prog), m_cubemap_texture(cubemap_texture)
{
    this->prepare();
}

SkyBoxRenderer::SkyBoxRenderer(ShaderProgram &shader_prog, const string cubemap_faces_filenames[6])
	: Renderer(shader_prog), m_cubemap_texture(cubemap_faces_filenames)
{
    this->prepare();
}

void SkyBoxRenderer::setCubeMapTexture(CubeMapTexture &new_cubemap_texture)
{
	m_cubemap_texture = new_cubemap_texture;
}

void SkyBoxRenderer::setCubeMapTexture(const string new_cubemap_faces_filenames[6])
{
	CubeMapTexture new_tex(new_cubemap_faces_filenames);
	m_cubemap_texture = new_tex;
}

void SkyBoxRenderer::prepare()
{
    // bind vertex data to shader
    m_cubemap_mesh.getVAO().bind();
    m_shader_prog.bindData(0, m_cubemap_mesh.getPositionsVBO(), 3);

    // bind cubemap sampler location
    m_shader_prog.use();
    m_shader_prog.setInt("env_map", 0);
}

void SkyBoxRenderer::render(const Camera &render_cam)
{
    // set success of depth test to <= (i.e. depth test passes when vals are <= depth buffer's content)
    glDepthFunc(GL_LEQUAL);
    
    // bind objects
    glActiveTexture(GL_TEXTURE0);
    m_cubemap_texture.bind();

    // set shader as active one
    m_shader_prog.use();

    // calc VP matrix & upload it to shader
    glm::mat4 vp_matrix = render_cam.getProjMatrix() * glm::mat4(glm::mat3(render_cam.getViewMatrix()));
    m_shader_prog.setMat4("vp_matrix", vp_matrix);

    // render skybox
    m_cubemap_mesh.render(); 

    // reset depth test
    glDepthFunc(GL_LESS);
}

CubeMapTexture &SkyBoxRenderer::getCubeMapTexture()
{
    return m_cubemap_texture;
}


// ------------------------------------
// --- (base) Ocean renderer ---

OceanRenderer::OceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr)
    : Renderer(shader_prog), m_ocean_mesh_ptr(ocean_mesh_ptr)
{
    this->prepare();
}

void OceanRenderer::prepare()
{
    // --- bind input mesh data
    m_ocean_mesh_ptr->getVAO().bind();
    m_shader_prog.bindData(0, m_ocean_mesh_ptr->getPositionsVBO(), 3);
    m_shader_prog.bindData(1, m_ocean_mesh_ptr->getTexCoordsVBO(), 2);

    // --- generate wave simulation parameters
    const int NUM_WAVES = 16; // !!! -- MUST be the SAME as in the VERTEX SHADER -- !!!
    float wavelengths[NUM_WAVES], freqs[NUM_WAVES], amplitudes[NUM_WAVES], phases[NUM_WAVES];
    glm::vec2 wavevecs[NUM_WAVES];
    float steepness[NUM_WAVES], steepness_norm_const = 0.0f;

    const float WAVELENGTH_RANGE = (2 * m_median_wavelength) - (m_median_wavelength / 2);
    const int DISTRIB_MAX = 1000;

    std::default_random_engine random_generator;
    std::uniform_int_distribution<int> uniform_distrib(0, DISTRIB_MAX);

    // gen steepness properties for the waves & normalise them
    {
        int random_int;
        for (int i = 0; i < NUM_WAVES; i++)
        {
            random_int = uniform_distrib(random_generator);
            steepness[i] = (random_int / (float)DISTRIB_MAX);
            steepness_norm_const += steepness[i];
        }
    }

    for (int i = 0; i < NUM_WAVES; i++)
    {
        // gen random wavelength in range [m_median_wavelength/2, 2*m_median_wavelength]
        int random_int = uniform_distrib(random_generator);
        wavelengths[i] = (m_median_wavelength / 2) + (random_int / (float) DISTRIB_MAX) * WAVELENGTH_RANGE;

        // calc wavenumber
        float wavenum = (2 * glm::pi<float>()) / wavelengths[i];

        // gen random wave vector at some angle from wind vector
        random_int = uniform_distrib(random_generator);
        float angle = -m_max_angle_deviation + (random_int / (float) DISTRIB_MAX) * 2 * m_max_angle_deviation;
        glm::mat2 rotation_matrix = glm::mat2(1.0f);
        rotation_matrix[0] = glm::vec2(glm::cos(angle), glm::sin(angle));   // 1st column
        rotation_matrix[1] = glm::vec2(-glm::sin(angle), glm::cos(angle));  // 2nd
        glm::vec2 direction = glm::normalize(rotation_matrix * m_wind_dir);
        
        wavevecs[i] = wavenum * direction;

        // gen freq based on dispersion relation
        freqs[i] = glm::sqrt(wavenum * 9.807);

        // gen amplitude based on steepness & wavenum
        amplitudes[i] = steepness[i] / (steepness_norm_const * wavenum);

        // gen random phase in range [0, 2*pi]
        random_int = uniform_distrib(random_generator);
        phases[i] = (random_int / (float)DISTRIB_MAX) * 2 * glm::pi<float>();
    }


    // --- set parameters arrays in shader
    m_shader_prog.use();
    m_shader_prog.setVec2Array("sim_wavevecs", wavevecs, NUM_WAVES);
    m_shader_prog.setFloatArray("sim_freqs", freqs, NUM_WAVES);
    m_shader_prog.setFloatArray("sim_amplitudes", amplitudes, NUM_WAVES);
    m_shader_prog.setFloatArray("sim_phases", phases, NUM_WAVES);

    // --- set water base colour params to default
    m_shader_prog.setVec3("water_base_colour", NereusConstants::DEFAULT_WATER_BASE_COLOUR);
}

void OceanRenderer::render(const Camera &render_cam)
{
    // transformation matrices
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(-m_ocean_width, -10.0f, -m_ocean_length));
    model_matrix = glm::scale(model_matrix, glm::vec3(
        m_ocean_width / (float)m_ocean_mesh_ptr->getGridWidth(),
        1.0f,
        m_ocean_length / (float)m_ocean_mesh_ptr->getGridLength()
    ));

    glm::mat4 vp_matrix = render_cam.getProjMatrix() * render_cam.getViewMatrix();

    // set matrices (uniforms) in shader
    m_shader_prog.use();
    m_shader_prog.setMat4("m_matrix", model_matrix);
    m_shader_prog.setMat4("vp_matrix", vp_matrix);

    // set other uniforms
    m_shader_prog.setFloat("time", glfwGetTime());
    m_shader_prog.setVec3("wc_camera_pos", render_cam.getPosition());

    // render mesh
    m_ocean_mesh_ptr->render();
}

void OceanRenderer::updateOceanMeshGrid(int new_grid_width, int new_grid_length)
{
    m_ocean_mesh_ptr->updateMeshGrid(new_grid_width, new_grid_length);
}


void OceanRenderer::setOceanWidth(int new_ocean_width)
{
    m_ocean_width = new_ocean_width;
}

void OceanRenderer::setOceanLength(int new_ocean_length)
{
    m_ocean_length = new_ocean_length;
}

void OceanRenderer::setWaterBaseColour(glm::vec3 &new_colour)
{
    m_shader_prog.use();
    m_shader_prog.setVec3("water_base_colour", new_colour);
}


// ------------------------------------
// --- Reflective Ocean renderer ---

ReflectiveOceanRenderer::ReflectiveOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr)
    : OceanRenderer(shader_prog, ocean_mesh_ptr), m_cubemap_texture(nullptr)
{
    this->prepare();
}

ReflectiveOceanRenderer::ReflectiveOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr, CubeMapTexture &skybox)
    : OceanRenderer(shader_prog, ocean_mesh_ptr), m_cubemap_texture(skybox)
{
    this->prepare();
}

void ReflectiveOceanRenderer::prepare()
{
    // will have called base renderer prepare method before this 
    // (since it's called from base renderer's constructor which is exec before this class's constructor)

    // --- bind cubemap sampler location 
    m_shader_prog.use();
    m_shader_prog.setInt("env_map", 0);  // at tex unit 0

    // set water base colour params to default
    m_shader_prog.setFloat("water_base_colour_amt", NereusConstants::DEFAULT_WATER_BASE_COLOUR_AMOUNT);
}

void ReflectiveOceanRenderer::render(const Camera &render_cam)
{
    // bind skybox texture
    glActiveTexture(GL_TEXTURE0);
    m_cubemap_texture.bind();

    // render using base renderer
    OceanRenderer::render(render_cam);
}

void ReflectiveOceanRenderer::setSkyboxTexture(CubeMapTexture &skybox)
{
    m_cubemap_texture = skybox;
}

void ReflectiveOceanRenderer::setWaterBaseColourAmount(float new_amt)
{
    m_shader_prog.use();
    m_shader_prog.setFloat("water_base_colour_amt", new_amt);
}


// ------------------------------------
// --- Refractive Ocean renderer ---

RefractiveOceanRenderer::RefractiveOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr)
    : OceanRenderer(shader_prog, ocean_mesh_ptr), m_texture_S(), m_fbo(m_texture_S)
{
    this->prepare();
}

void RefractiveOceanRenderer::prepare()
{
    // will have called base renderer prepare method before this 
    // (since it's called from base renderer's constructor which is exec before this class's constructor)

    // bind texture S's sampler location 
    m_shader_prog.use();
    m_shader_prog.setInt("tex_S", 0);  // at tex unit 0

    // set water base colour params to default
    m_shader_prog.setFloat("water_base_colour_amt", NereusConstants::DEFAULT_WATER_BASE_COLOUR_AMOUNT);

    // set viewport dimensions
    m_shader_prog.setVec2("viewport_dimensions", 
        glm::vec2(NereusConstants::DEFAULT_WINDOW_WIDTH, NereusConstants::DEFAULT_WINDOW_HEIGHT)
    );
}

void RefractiveOceanRenderer::render(const Camera &render_cam)
{
    // bind texture S
    glActiveTexture(GL_TEXTURE0);
    m_texture_S.bind();

    // render using base renderer
    OceanRenderer::render(render_cam);
}

void RefractiveOceanRenderer::bindFBO()
{
    m_fbo.bind();
}

void RefractiveOceanRenderer::unbindFBO()
{
    m_fbo.unbind();
}

Texture2D &RefractiveOceanRenderer::getTextureS()
{
    return m_texture_S;
}

void RefractiveOceanRenderer::setWaterBaseColourAmount(float new_amt)
{
    m_shader_prog.use();
    m_shader_prog.setFloat("water_base_colour_amt", new_amt);
}


// ------------------------------------
// --- Full Ocean renderer (Reflection & Refraction; Fresnel effect) ---

FullOceanRenderer::FullOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr)
    : OceanRenderer(shader_prog, ocean_mesh_ptr), m_cubemap_texture(nullptr), m_texture_S(), m_fbo(m_texture_S)
{
    this->prepare();
}

FullOceanRenderer::FullOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr, CubeMapTexture &skybox)
    : OceanRenderer(shader_prog, ocean_mesh_ptr), m_cubemap_texture(skybox), m_texture_S(), m_fbo(m_texture_S)
{
    this->prepare();
}

void FullOceanRenderer::prepare()
{
    // will have called base renderer prepare method before this 
    // (since it's called from base renderer's constructor which is exec before this class's constructor)

    // --- for reflection ---
    // set cubemap sampler location 
    m_shader_prog.use();
    m_shader_prog.setInt("env_map", 0);  // at tex unit 0


    // --- for refraction ---
    // set texture S's sampler location 
    m_shader_prog.use();
    m_shader_prog.setInt("tex_S", 1);  // at tex unit 1
    // set water base colour params to default
    m_shader_prog.setFloat("water_base_colour_amt", NereusConstants::DEFAULT_WATER_BASE_COLOUR_AMOUNT);

    // set viewport dimensions
    m_shader_prog.setVec2("viewport_dimensions", 
        glm::vec2(NereusConstants::DEFAULT_WINDOW_WIDTH, NereusConstants::DEFAULT_WINDOW_HEIGHT)
    );

    // --- for fresnel effect ---
    m_shader_prog.setFloat("fresnel_F_0", m_FRESNEL_F0);
}

void FullOceanRenderer::render(const Camera &render_cam)
{
    // --- for reflection ---
    // bind skybox texture
    glActiveTexture(GL_TEXTURE0);
    m_cubemap_texture.bind();

    // --- for refraction ---
    // bind texture S
    glActiveTexture(GL_TEXTURE1);
    m_texture_S.bind();

    // --- render using base renderer ---
    OceanRenderer::render(render_cam);
}

void FullOceanRenderer::bindFBO()
{
    m_fbo.bind();
}

void FullOceanRenderer::unbindFBO()
{
    m_fbo.unbind();
}

Texture2D &FullOceanRenderer::getTextureS()
{
    return m_texture_S;
}

void FullOceanRenderer::setSkyboxTexture(CubeMapTexture &skybox)
{
    m_cubemap_texture = skybox;
}

void FullOceanRenderer::setWaterBaseColourAmount(float new_amt)
{
    m_shader_prog.use();
    m_shader_prog.setFloat("water_base_colour_amt", new_amt);
}


// ------------------------------------
// --- Seabed renderer ---

SeabedRenderer::SeabedRenderer(ShaderProgram &shader_prog, Texture2D &perlin_tex)
    : Renderer(shader_prog), m_perlin_texture(perlin_tex), 
    m_seabed_texture(), m_use_seabed_texture(false),
    m_seabed_mesh(NereusConstants::DEFAULT_SEABED_GRID_WIDTH, NereusConstants::DEFAULT_SEABED_GRID_LENGTH)
{
    m_seabed_mesh.initialise();
    this->prepare();
}

SeabedRenderer::SeabedRenderer(ShaderProgram &shader_prog, Texture2D &perlin_tex, Texture2D &seabed_tex)
    : Renderer(shader_prog), m_perlin_texture(perlin_tex), 
    m_seabed_texture(seabed_tex), m_use_seabed_texture(true),
    m_seabed_mesh(NereusConstants::DEFAULT_SEABED_GRID_WIDTH, NereusConstants::DEFAULT_SEABED_GRID_LENGTH)
{
    m_seabed_mesh.initialise();
    this->prepare();
}

void SeabedRenderer::prepare()
{
    // --- bind input mesh data
    m_seabed_mesh.getVAO().bind();
    m_shader_prog.bindData(0, m_seabed_mesh.getPositionsVBO(), 3);
    m_shader_prog.bindData(1, m_seabed_mesh.getTexCoordsVBO(), 2);

    // --- bind textures 
    m_shader_prog.use();
    m_shader_prog.setInt("perlin_tex", 0);  // at tex unit 1
    m_shader_prog.setInt("seabed_tex", 1);  // at tex unit 2
    m_shader_prog.setInt("use_seabed_tex", 0);

    if (m_use_seabed_texture)
    {
        m_shader_prog.setInt("use_seabed_tex", 1);
    }
    
}

void SeabedRenderer::render(const Camera &render_cam)
{
    // transformation matrices
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(
        -m_seabed_width + NereusConstants::SEABED_EXTENSION_FROM_OCEAN, 
        -10.0f - NereusConstants::SEABED_DEPTH_BELOW_OCEAN,
        -m_seabed_length + NereusConstants::SEABED_EXTENSION_FROM_OCEAN
    ));
    model_matrix = glm::scale(model_matrix, glm::vec3(
        m_seabed_width / (float)m_seabed_mesh.getGridWidth(),
        1.0f,
        m_seabed_length / (float)m_seabed_mesh.getGridLength()
    ));

    glm::mat4 vp_matrix = render_cam.getProjMatrix() * render_cam.getViewMatrix();

    // set matrices (uniforms) in shader
    m_shader_prog.use();
    m_shader_prog.setMat4("m_matrix", model_matrix);
    m_shader_prog.setMat4("vp_matrix", vp_matrix);

    // set other uniforms
    m_shader_prog.setVec3("wc_camera_pos", render_cam.getPosition());

    // bind perlin noise texture
    glActiveTexture(GL_TEXTURE0);
    m_perlin_texture.bind();

    // bind seabed texture
    if (m_use_seabed_texture)
    {
        glActiveTexture(GL_TEXTURE1);
        m_seabed_texture.bind();
    }

    // render mesh
    m_seabed_mesh.render();
}

void SeabedRenderer::updateSeabedMeshGrid(int new_grid_width, int new_grid_length)
{
    m_seabed_mesh.updateMeshGrid(new_grid_width, new_grid_length);
}

void SeabedRenderer::setSeabedWidth(int new_seabed_width)
{
    m_seabed_width = new_seabed_width;
}

void SeabedRenderer::setSeabedLength(int new_seabed_length)
{
    m_seabed_length = new_seabed_length;
}

void SeabedRenderer::setPerlinTexture(Texture2D &perlin_tex)
{
    m_perlin_texture = perlin_tex;
}

void SeabedRenderer::setSeabedTexture(Texture2D &seabed_tex)
{
    m_seabed_texture = seabed_tex;
    if (m_use_seabed_texture != true)
    {
        m_use_seabed_texture = true;
        m_shader_prog.use();
        m_shader_prog.setInt("use_seabed_tex", 1);
    }
}

void SeabedRenderer::removeSeabedTexture()
{
    m_use_seabed_texture = false;
    m_shader_prog.use();
    m_shader_prog.setInt("use_seabed_tex", 0);
}


// ------------------------------------
// --- Screen Quad renderer (for visual debugging) ---

ScreenQuadRenderer::ScreenQuadRenderer(ShaderProgram &shader_prog, Texture2D &screen_tex)
    : Renderer(shader_prog), m_quad_mesh(), m_screen_tex(screen_tex)
{
    m_quad_mesh.initialise();
    this->prepare();
}

void ScreenQuadRenderer::prepare()
{
    // bind input mesh data
    m_quad_mesh.getVAO().bind();
    m_shader_prog.bindData(0, m_quad_mesh.getPositionsVBO(), 2);
    m_shader_prog.bindData(1, m_quad_mesh.getTexCoordsVBO(), 2);

    // set texture
    m_shader_prog.setInt("screen_tex", 0);
}

void ScreenQuadRenderer::render(const Camera &render_cam)
{
    m_shader_prog.use();

    // bind screeen texture
    glActiveTexture(GL_TEXTURE0);
    m_screen_tex.bind();

    // render
    m_quad_mesh.render();
}
