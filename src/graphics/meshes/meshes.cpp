#include "meshes.h"
#include <vector>

// ------------------------------------
// Mesh

// Create the appropriate buffer objects for the mesh attributes
Mesh::Mesh() : m_vao(), m_positionsVBO(), m_normalsVBO(), m_texCoordsVBO(), m_ebo(), m_vertexCount(0)
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
	m_vertexCount = indices.size();

	this->loadDataToGPU(positions, normals, texCoords, indices);
}

// Load array data onto GPU buffer objects
void Mesh::loadDataToGPU(const std::vector<float> &positions,
						const std::vector<float> &normals, 
						const std::vector<float> &texCoords, 
						const std::vector<int> &indices)
{
	// --- bind VAO ---
	m_vao.bind();

	// --- load vertex positions ---
	m_positionsVBO.setData<float>(positions);

	// --- load vertex normals ---
	m_normalsVBO.setData<float>(normals);

	// --- load texture coordinates ---
	m_texCoordsVBO.setData<float>(texCoords);

	// --- load vertex indices ---
	m_ebo.setData<int>(indices);
}

// Destructor (will automatically call destructor of member vars?) TODO check
Mesh::~Mesh()
{

}

// Bind VAO & do a draw call (using the currently active shader program)
void Mesh::render()
{
	m_vao.bind();
	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);
	m_vao.unbind();
}


// Getters

VAO &Mesh::getVAO()
{
	return m_vao;
}

VBO &Mesh::getPositionsVBO()
{
	return m_positionsVBO;
}

VBO &Mesh::getNormalsVBO()
{
	return m_normalsVBO;
}

VBO &Mesh::getTexCoordsVBO()
{
	return m_texCoordsVBO;
}


// ------------------------------------
// CubeMapMesh

CubeMapMesh *CubeMapMesh::s_instance = nullptr;

CubeMapMesh &CubeMapMesh::getInstance()
{
    if (CubeMapMesh::s_instance == nullptr)
    {
        CubeMapMesh::s_instance = new CubeMapMesh();
        CubeMapMesh::s_instance->initialise();
    }

    return *CubeMapMesh::s_instance;
}

CubeMapMesh::CubeMapMesh()
{

}

CubeMapMesh::~CubeMapMesh()
{
    CubeMapMesh::s_instance = nullptr;
}

void CubeMapMesh::initPositions(std::vector<float> &pos)
{
    pos = {
		// vertices             index
		-0.5f, -0.5f, -0.5f,    // 0
		0.5f, -0.5f, -0.5f,     // 1
		0.5f, 0.5f, -0.5f,      // 2
		-0.5f, 0.5f, -0.5f,     // 3
		-0.5f, -0.5f, 0.5f,     // 4
		0.5f, 0.5f, 0.5f,       // 5
		0.5f, -0.5f, 0.5f,      // 6
		-0.5f, 0.5f, 0.5f       // 7
    };
}

void CubeMapMesh::initNormals(std::vector<float> &normals)
{
    // dummy
    normals = {0.0f};
}

void CubeMapMesh::initTexCoords(std::vector<float> &texCoords)
{
    // dummy
    texCoords = {0.0f};
}

void CubeMapMesh::initIndices(std::vector<int> &indices)
{
    indices = {
		0, 1, 2, 2, 3, 0,   // back face
		4, 5, 6, 5, 4, 7,   // front face
		7, 0, 3, 0, 7, 4,   // left face
		5, 2, 1, 1, 6, 5,   // right face
		0, 6, 1, 6, 0, 4,   // bottom face
		3, 2, 5, 5, 7, 3    // top face
    };
}
