#ifndef MESHES
#define MESHES
#pragma once

#include <glad/glad.h>
#include "../buffers.h"

class Mesh
{
private:
	VAO vao;
	VBO positionsVBO;
	VBO normalsVBO;
	VBO texCoordsVBO;
	EBO ebo;
	int vertexCount;

	void Mesh::loadDataToGPU(const std::vector<float> &positions, 
							const std::vector<float> &normals, 
							const std::vector<float> &texCoords, 
							const std::vector<int> &indices);
	
	// Initialise the data arrays for positions, normals, texture coordinates & vertex indices
	virtual void initPositions(std::vector<float> &pos) = 0 {};
	virtual void initNormals(std::vector<float> &normals) = 0 {};
	virtual void initTexCoords(std::vector<float> &texCoords) = 0 {};
	virtual void initIndices(std::vector<int> &indices) = 0 {};

	void initialise();

public:
	Mesh();
	void render();
};

#endif