#ifndef MESHES
#define MESHES
#pragma once

#include <glad/glad.h>

class Mesh
{
private:
	VAO vao;
	VBO positionsVBO;
	VBO normalsVBO;
	VBO texCoordsVBO;
	EBO ebo;
	int vertexCount;

	void loadDataToGPU(const (&float[]) positions, const (&float[]) normals, const (&float[]) texCoords, const (&int[]) indices);
	
	// Initialise the data arrays for positions, normals, texture coordinates & vertex indices
	void initPositions((&float[]) pos) = 0;
	void initNormals((&float[]) normals) = 0;
	void initTexCoords((&float[]) texCoords) = 0;
	void initIndices((&int[]) indices) = 0;

	void initialise();

public:
	void render();
};

#endif