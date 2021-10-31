#include "meshes.h"
#include <vector>

// Create the appropriate buffer objects for the mesh attributes
Mesh::Mesh() : vao(), positionsVBO(), normalsVBO(), texCoordsVBO(), ebo()
{
	Mesh::initialise();
}

// Create and load the mesh data to GPU buffers
void Mesh::initialise()
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<int> indices;

	Mesh::initPositions(positions);
	Mesh::initNormals(normals);
	Mesh::initTexCoords(texCoords);
	Mesh::initIndices(indices);
	Mesh::vertexCount = sizeof(indices) / sizeof(int);

	Mesh::loadDataToGPU(positions, normals, texCoords, indices);
}

// Load array data onto GPU buffer objects
void Mesh::loadDataToGPU(const std::vector<float> &positions,
						const std::vector<float> &normals, 
						const std::vector<float> &texCoords, 
						const std::vector<int> &indices)
{
	// --- bind VAO ---
	Mesh::vao.bind();

	// --- load vertex positions ---
	Mesh::positionsVBO.setData<float>(positions);

	// --- load vertex normals ---
	Mesh::normalsVBO.setData<float>(normals);

	// --- load texture coordinates ---
	Mesh::texCoordsVBO.setData<float>(texCoords);

	// --- load vertex indices ---
	Mesh::texCoordsVBO.setData<int>(indices);
}

void Mesh::render()
{
	Mesh::vao.bind();
	glDrawElements(GL_TRIANGLES, Mesh::vertexCount, GL_UNSIGNED_INT, 0);
	Mesh::vao.unbind();
}