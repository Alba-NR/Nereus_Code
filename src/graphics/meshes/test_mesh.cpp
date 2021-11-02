
#include "test_mesh.h"
#include <iostream>

void TestMesh::initPositions(std::vector<float> &pos)
{
	pos = {
		0.5f,  0.5f, 0.0f,		// top right
		0.5f, -0.5f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		// bottom left
		-0.5f,  0.5f, 0.0f		// top left 
	};
}
void TestMesh::initNormals(std::vector<float> &normals)
{
	// dummy
	normals = { 
		0.0f,  0.0f, 0.0f,
		0.0f,  0.0f, 0.0f,
		0.0f,  0.0f, 0.0f,
		0.0f,  0.0f, 0.0f
	};
}
void TestMesh::initTexCoords(std::vector<float> &texCoords)
{
	// dummy
	texCoords = { 
		0.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  0.0f
	};
}
void TestMesh::initIndices(std::vector<int> &indices)
{
	indices = {
		0, 1, 3,
		1, 2, 3
	};
}