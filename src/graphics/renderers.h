#ifndef RENDERERS
#define RENDERERS
#pragma once

#include "shaders.h"
#include "meshes.h"
#include "textures.h"
#include "buffers.h"
#include "camera.h"
#include "../main/constants.h"

#include <memory>


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
// --- (base) Ocean renderer ---
class OceanRenderer : public Renderer
{
private:
	std::shared_ptr<GridMesh> m_ocean_mesh_ptr;

	int m_ocean_width = NereusConstants::DEFAULT_OCEAN_WIDTH;
	int m_ocean_length = NereusConstants::DEFAULT_OCEAN_LENGTH;

	const float m_median_wavelength = 20.0f;
	const glm::vec2 m_wind_dir = glm::normalize(glm::vec2(2.0f, 3.0f));
	const float m_max_angle_deviation = glm::radians(30.0f);

protected:
	virtual void prepare();

public:
	OceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr);

	virtual void render(const Camera &render_cam);

	void updateOceanMeshGrid(int new_grid_width, int new_grid_length);
	void setOceanWidth(int new_ocean_width);
	void setOceanLength(int new_ocean_length);

	void setWaterBaseColour(glm::vec3 &new_colour);
};

// --- Reflective Ocean renderer ---
class ReflectiveOceanRenderer : public OceanRenderer
{
private:
	CubeMapTexture m_cubemap_texture;

	void prepare();

public:
	ReflectiveOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr);
	ReflectiveOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr, CubeMapTexture &skybox);

	void render(const Camera &render_cam);

	void setSkyboxTexture(CubeMapTexture &skybox);
	void setWaterBaseColourAmount(float new_amt);
};

// --- Refractive Ocean renderer ---
class RefractiveOceanRenderer : public OceanRenderer
{
private:
	Texture2D m_texture_S;
	FBO m_fbo;

	void prepare();

public:
	RefractiveOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr);

	void render(const Camera &render_cam);

	void bindFBO();
	void unbindFBO();

	Texture2D &getTextureS();

	void setWaterBaseColourAmount(float new_amt);
};


// --- Full Ocean renderer (Reflection & Refraction; Fresnel effect) ---
class FullOceanRenderer : public OceanRenderer
{
private:
	// for reflection
	CubeMapTexture m_cubemap_texture;
	// for refraction
	Texture2D m_texture_S;
	FBO m_fbo;
	// for fresnel effect
	const float m_FRESNEL_F0 = glm::pow((
		(NereusConstants::WATER_REFRACTIVE_INDEX - NereusConstants::AIR_REFRACTIVE_INDEX)
		/ (NereusConstants::WATER_REFRACTIVE_INDEX + NereusConstants::AIR_REFRACTIVE_INDEX)
	), 2);

	void prepare();

public:
	FullOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr);
	FullOceanRenderer(ShaderProgram &shader_prog, std::shared_ptr<GridMesh> ocean_mesh_ptr, CubeMapTexture &skybox);

	void render(const Camera &render_cam);

	void bindFBO();
	void unbindFBO();

	Texture2D &getTextureS();

	void setSkyboxTexture(CubeMapTexture &skybox);
	void setWaterBaseColourAmount(float new_amt);
};


// ------------------------------------
// --- Seabed renderer ---
class SeabedRenderer : public Renderer
{
private:
	GridMesh m_seabed_mesh;

	int m_seabed_width = NereusConstants::DEFAULT_OCEAN_WIDTH + NereusConstants::SEABED_EXTENSION_FROM_OCEAN;
	int m_seabed_length = NereusConstants::DEFAULT_OCEAN_LENGTH + NereusConstants::SEABED_EXTENSION_FROM_OCEAN;

	Texture2D m_perlin_texture;
	bool m_use_seabed_texture;
	Texture2D m_seabed_texture;

	void prepare();

public:
	SeabedRenderer(ShaderProgram &shader_prog, Texture2D &perlin_tex);
	SeabedRenderer(ShaderProgram &shader_prog, Texture2D &perlin_tex, Texture2D &seabed_tex);

	void render(const Camera &render_cam);

	void updateSeabedMeshGrid(int new_grid_width, int new_grid_length);
	void setSeabedWidth(int new_seabed_width);
	void setSeabedLength(int new_seabed_length);

	void setPerlinTexture(Texture2D &perlin_tex);
	void setSeabedTexture(Texture2D &seabed_tex);
	void removeSeabedTexture();
};


// ------------------------------------
// --- Screen quad renderer (for visual debugging) ---
class ScreenQuadRenderer : public Renderer
{
private:
	ScreenQuadMesh m_quad_mesh;
	Texture2D m_screen_tex;
	void prepare();

public:
	ScreenQuadRenderer(ShaderProgram &shader_prog, Texture2D &screen_tex);

	void render(const Camera &render_cam);
};
#endif