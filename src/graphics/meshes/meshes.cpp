#include "meshes.h"
#include <vector>

// Create the appropriate buffer objects for the mesh attributes
Mesh::Mesh() : vao(), positionsVBO(), normalsVBO(), texCoordsVBO(), ebo(), vertexCount(0)
{
}

// Create and load the mesh data to GPU buffers
void Mesh::initialise()
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<int> indices;

	this->initPositions(positions);
	this->initNormals(normals);
	this->initTexCoords(texCoords);
	this->initIndices(indices);
	this->vertexCount = indices.size();

	this->loadDataToGPU(positions, normals, texCoords, indices);
}

// Load array data onto GPU buffer objects
void Mesh::loadDataToGPU(const std::vector<float> &positions,
						const std::vector<float> &normals, 
						const std::vector<float> &texCoords, 
						const std::vector<int> &indices)
{
	// --- bind VAO ---
	this->vao.bind();

	// --- load vertex positions ---
	this->positionsVBO.setData<float>(positions);

	// --- load vertex normals ---
	this->normalsVBO.setData<float>(normals);

	// --- load texture coordinates ---
	this->texCoordsVBO.setData<float>(texCoords);

	// --- load vertex indices ---
	this->ebo.setData<int>(indices);
}

// Destructor (will automatically call destructor of member vars?) TODO check
Mesh::~Mesh()
{

}

// Bind VAO & do a draw call (using the currently active shader program)
void Mesh::render()
{
	this->vao.bind();
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, 0);
	this->vao.unbind();
}


// Getters

VAO &Mesh::getVAO()
{
	return this->vao;
}

VBO &Mesh::getPositionsVBO()
{
	return this->positionsVBO;
}

VBO &Mesh::getNormalsVBO()
{
	return this->normalsVBO;
}

VBO &Mesh::getTexCoordsVBO()
{
	return this->texCoordsVBO;
}
