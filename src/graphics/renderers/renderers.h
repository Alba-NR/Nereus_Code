#ifndef RENDERERS
#define RENDERERS
#pragma once

#include "../shaders.h"
#include "../meshes/meshes.h"
#include "../textures.h"
#include "../camera.h"

// --- Renderer interface ---
class Renderer
{
protected:
	ShaderProgram m_shader_prog;

public:
	Renderer(ShaderProgram &shader_prog);
	virtual void render(const Camera &render_cam) = 0;
};


// --- Skybox renderer ---
class SkyBoxRenderer : public Renderer
{
private:
	CubeMapMesh m_cubemap_mesh = CubeMapMesh::getInstance();
	CubeMapTexture m_cubemap_texture;

public:
	SkyBoxRenderer(ShaderProgram &shader_prog);
	SkyBoxRenderer(ShaderProgram &shader_prog, CubeMapTexture &cubemap_texture);
	SkyBoxRenderer(ShaderProgram &shader_prog, const string cubemap_faces_filenames[6]);

	void setCubeMapTexture(CubeMapTexture &new_cubemap_texture);
	void setCubeMapTexture(const string new_cubemap_faces_filenames[6]);

	void prepare();
	void render(const Camera &render_cam);
};
#endif