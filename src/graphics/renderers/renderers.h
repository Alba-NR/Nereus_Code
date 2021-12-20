#ifndef RENDERERS
#define RENDERERS
#pragma once

#include "../shaders.h"
#include "../meshes/meshes.h"
#include "../textures.h"
#include "../camera.h"
#include "../../main/constants.h"


// ------------------------------------
// --- Renderer interface ---
class Renderer
{
protected:
	ShaderProgram m_shader_prog;

public:
	Renderer(ShaderProgram &shader_prog);
	virtual void render(const Camera &render_cam) = 0;
};


// ------------------------------------
// --- Skybox renderer ---
class SkyBoxRenderer : public Renderer
{
private:
	CubeMapMesh m_cubemap_mesh = CubeMapMesh::getInstance();
	CubeMapTexture m_cubemap_texture;

	void prepare();

public:
	SkyBoxRenderer(ShaderProgram &shader_prog);
	SkyBoxRenderer(ShaderProgram &shader_prog, CubeMapTexture &cubemap_texture);
	SkyBoxRenderer(ShaderProgram &shader_prog, const string cubemap_faces_filenames[6]);

	void setCubeMapTexture(CubeMapTexture &new_cubemap_texture);
	void setCubeMapTexture(const string new_cubemap_faces_filenames[6]);

	void render(const Camera &render_cam);

	CubeMapTexture &getCubeMapTexture();
};


// ------------------------------------
// --- Ocean renderer ---
class OceanRenderer : public Renderer
{
private:
	OceanMesh m_ocean_mesh = OceanMesh::getInstance();

	int m_ocean_width = NereusConstants::DEFAULT_OCEAN_WIDTH;
	int m_ocean_length = NereusConstants::DEFAULT_OCEAN_LENGTH;

	const float m_median_wavelength = 20.0f;
	const glm::vec2 m_wind_dir = glm::normalize(glm::vec2(2.0f, 3.0f));
	const float m_max_angle_deviation = glm::radians(30.0f);

	CubeMapTexture m_cubemap_texture;

	void prepare();

public:
	OceanRenderer(ShaderProgram &shader_prog);
	OceanRenderer(ShaderProgram &shader_prog, CubeMapTexture &skybox);


	void render(const Camera &render_cam);

	void updateOceanMeshGrid(int new_grid_width, int new_grid_length);
	void setOceanWidth(int new_ocean_width);
	void setOceanLength(int new_ocean_length);

	void setSkyboxTexture(CubeMapTexture &skybox);
};
#endif