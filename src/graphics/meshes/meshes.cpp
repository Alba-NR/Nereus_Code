# include "meshes.h"

// Create the appropriate buffer objects and load the mesh data to GPU buffers
void Mesh::initialise()
{
	float[] positions;
	float[] normals;
	float[] texCoords;
	int[] indices;

	Mesh::initPositions(positions);
	Mesh::initNormals(normals);
	Mesh::initTexCoords(texCoords);
	Mesh::initIndices(indices);
	Mesh::vertexCount = sizeof(indices) / sizeof(int);

	Mesh::loadDataToGPU(positions, normals, texCoords, indices);
}

// Load array data onto GPU buffer objects
void Mesh::loadDataToGPU(const (&float[]) positions, const (&float[]) normals, const (&float[]) texCoords, const (&int[]) indices)
{
	// --- VAO creation & bind ---
	VAO Mesh::vao();
	Mesh::vao.bind();

	// --- load vertex positions ---
	VBO Mesh::positionsVBO();
	Mesh::positionsVBO.setData<float>(positions);

	// --- load vertex normals ---
	VBO Mesh::normalsVBO();
	Mesh::normalsVBO.setData<float>(normals);

	// --- load texture coordinates ---
	VBO Mesh::texCoordsVBO();
	Mesh::texCoordsVBO.setData<float>(texCoords);

	// --- load vertex indices ---
	EBO Mesh::ebo();
	Mesh::texCoordsVBO.setData<int>(indices);
}

void Mesh::render()
{
	Mesh::vao.bind();
	glDrawElements(GL_TRIANGLES, Mesh::vertexCount, GL_UNSIGNED_INT, 0);
	Mesh::vao.unbind();
}