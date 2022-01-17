#ifndef MESHES
#define MESHES
#pragma once

#include <glad/glad.h>
#include "buffers.h"

// --- Mesh abstract class ---
class Mesh
{
private:
	VAO m_vao;
	VBO m_positionsVBO;
	VBO m_normalsVBO;
	VBO m_texCoordsVBO;
	EBO m_ebo;
	int m_vertexCount;

	void Mesh::loadDataToGPU(const std::vector<float> &positions, 
							const std::vector<float> &normals, 
							const std::vector<float> &texCoords, 
							const std::vector<int> &indices);
	
	// Initialise the data arrays for positions, normals, texture coordinates & vertex indices
	virtual void initPositions(std::vector<float> &pos) = 0;
	virtual void initNormals(std::vector<float> &normals) = 0;
	virtual void initTexCoords(std::vector<float> &texCoords) = 0;
	virtual void initIndices(std::vector<int> &indices) = 0;

protected:
	Mesh();

public:
	virtual ~Mesh();
	void render();

	void initialise();

	VAO &getVAO();
	VBO &getPositionsVBO();
	VBO &getNormalsVBO();
	VBO &getTexCoordsVBO();
};


// --- Simple test mesh for initial debugging ---
class TestMesh : public Mesh
{
private:
	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);
};

// --- Cubemap mesh ---
class CubeMapMesh : public Mesh
{
private:
	static CubeMapMesh *s_instance;

	CubeMapMesh();

	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);

public:
	static CubeMapMesh &getInstance();
	~CubeMapMesh();
};


// --- Grid mesh (for Ocean & Terrain) ---
class GridMesh : public Mesh
{
private:
	int m_grid_width;
	int m_grid_length;

	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);

public:
	GridMesh(int grid_width, int grid_length);

	void updateMeshGrid(int new_grid_width, int new_grid_length);

	int getGridWidth() const;
	int getGridLength() const;
};


// --- Screen Quad mesh (for visual debugging) ---
class ScreenQuadMesh : public Mesh
{
private:
	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);

public:
	ScreenQuadMesh();
};
#endif