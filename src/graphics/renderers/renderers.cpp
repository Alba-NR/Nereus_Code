
#include "renderers.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


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
    //glActiveTexture(GL_TEXTURE0);
    m_cubemap_texture.bind();

    // bind vertex data to shader
    m_shader_prog.use();

    // calc VP matrix & upload it to shader
    glm::mat4 vp_matrix = render_cam.getProjMatrix() * glm::mat4(glm::mat3(render_cam.getViewMatrix()));
    m_shader_prog.setMat4("vp_matrix", vp_matrix);

    // render skybox
    m_cubemap_mesh.render(); 

    // reset depth test
    glDepthFunc(GL_LESS);
}


// ------------------------------------
// --- Ocean renderer ---

OceanRenderer::OceanRenderer(ShaderProgram &shader_prog)
    : Renderer(shader_prog)
{
    this->prepare();
}

void OceanRenderer::prepare()
{
    m_ocean_mesh.initialise();
    m_ocean_mesh.getVAO().bind();
    m_shader_prog.bindData(0, m_ocean_mesh.getPositionsVBO(), 3);
    m_shader_prog.bindData(1, m_ocean_mesh.getTexCoordsVBO(), 2);
}

void OceanRenderer::render(const Camera &render_cam)
{
    // transformation matrices
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(-m_ocean_width / 2, 0.0f, -m_ocean_length / 2));
    model_matrix = glm::scale(model_matrix, glm::vec3(
        m_ocean_width / (float)m_ocean_mesh.getGridWidth(),
        1.0f,
        m_ocean_length / (float)m_ocean_mesh.getGridLength()
    ));

    glm::mat4 vp_matrix = render_cam.getProjMatrix() * render_cam.getViewMatrix();

    // calc matrix to transform normals from object to world coords
    glm::mat4 normals_matrix = glm::mat4(1.0f);
    normals_matrix = glm::transpose(glm::inverse(model_matrix));

    // set matrices (uniforms) in shader
    m_shader_prog.use();
    m_shader_prog.setMat4("m_matrix", model_matrix);
    m_shader_prog.setMat4("vp_matrix", vp_matrix);
    m_shader_prog.setMat4("normals_matrix", normals_matrix);

    // set other uniforms
    m_shader_prog.setFloat("time", glfwGetTime());
    m_shader_prog.setVec3("wc_camera_pos", render_cam.getPosition());

    // render mesh
    m_ocean_mesh.render();
}

void OceanRenderer::updateOceanMeshGrid(int new_grid_width, int new_grid_length)
{
    m_ocean_mesh.updateMeshGrid(new_grid_width, new_grid_length);
}


void OceanRenderer::setOceanWidth(int new_ocean_width)
{
    m_ocean_width = new_ocean_width;
}

void OceanRenderer::setOceanLength(int new_ocean_length)
{
    m_ocean_length = new_ocean_length;
}