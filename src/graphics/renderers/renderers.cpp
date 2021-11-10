
#include "renderers.h"

// --- Renderer interface ---
Renderer::Renderer(ShaderProgram &shader_program) : m_shader_prog(shader_program)
{

}

// --- Skybox renderer ---

SkyBoxRenderer::SkyBoxRenderer(ShaderProgram &shader_prog) 
: Renderer(shader_prog), m_cubemap_texture(nullptr)
{

}

SkyBoxRenderer::SkyBoxRenderer(ShaderProgram &shader_prog, CubeMapTexture &cubemap_texture)
	: Renderer(shader_prog), m_cubemap_texture(cubemap_texture)
{

}

SkyBoxRenderer::SkyBoxRenderer(ShaderProgram &shader_prog, const string cubemap_faces_filenames[6])
	: Renderer(shader_prog), m_cubemap_texture(cubemap_faces_filenames)
{

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