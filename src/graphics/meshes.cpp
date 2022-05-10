#include "meshes.h"
#include "../main/constants.h"

#include <vector>

#define NOMINMAX // to disable 'max' macro from minwindef.h #included indirectly

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

// Destructor (will automatically call destructor of member vars?)
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
// TestMesh
void TestMesh::initPositions(std::vector<float> &pos)
{
	// a square
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


// ------------------------------------
// GridMesh

GridMesh::GridMesh(int grid_width, int grid_height)
	: m_grid_width(std::max(grid_width, 1)), m_grid_length(std::max(grid_height, 1))
{
}

void GridMesh::initPositions(std::vector<float> &pos)
{
	for (int j = 0; j < m_grid_length + 1; j++)
	{
		for (int i = 0; i < m_grid_width + 1; i++)
		{
			pos.push_back(i);
			pos.push_back(0);
			pos.push_back(j);
		}
	}
}

void GridMesh::initNormals(std::vector<float> &normals)
{
	for (int j = 0; j < m_grid_length + 1; j++)
	{
		for (int i = 0; i < m_grid_width + 1; i++)
		{
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}
	}
}

void GridMesh::initTexCoords(std::vector<float> &texCoords)
{
	for (int j = 0; j < m_grid_length + 1; j++)
	{
		for (int i = 0; i < m_grid_width + 1; i++)
		{
			texCoords.push_back(i / (float) m_grid_width);  // u
			texCoords.push_back(j / (float) m_grid_length); // v
		}
	}
}

void GridMesh::initIndices(std::vector<int> &indices)
{
	int stride_h = (m_grid_width + 1);

	for (int j = 0; j < m_grid_length; j++)
	{
		for (int i = 0; i < m_grid_width; i++)
		{
			// bottom triangle
			indices.push_back(stride_h * j + i);
			indices.push_back(stride_h * (j + 1) + i);
			indices.push_back(stride_h * j + i + 1);

			// top triangle
			indices.push_back(stride_h * (j + 1) + i);
			indices.push_back(stride_h * (j + 1) + i + 1);
			indices.push_back(stride_h * j + i + 1);
		}
	}
}

// Updates the OpenGL buffer data to that for a grid with the new dimensions/resolution
void GridMesh::updateMeshGrid(int new_grid_width, int new_grid_length)
{
	m_grid_width = new_grid_width;
	m_grid_length = new_grid_length;

	this->initialise();
}

int GridMesh::getGridWidth() const
{
	return m_grid_width;
}

int GridMesh::getGridLength() const
{
	return m_grid_length;
}


// ------------------------------------
// ScreenQuadMesh

ScreenQuadMesh::ScreenQuadMesh()
{
}

void ScreenQuadMesh::initPositions(std::vector<float> &pos)
{
	pos = {
		1.0f,  1.0f,		// top right
		1.0f, -1.0f,		// bottom right
		-1.0f, -1.0,		// bottom left
		-1.0f,  1.0f		// top left 
	};
}

void ScreenQuadMesh::initNormals(std::vector<float> &normals)
{
	// dummy
	normals = { 0.0f };
}

void ScreenQuadMesh::initTexCoords(std::vector<float> &texCoords)
{
	texCoords = {
		1.0f,  1.0f,		// top right (OpenGL origin @ bottom left corner)
		1.0f,  0.0f,		// bottom right
		0.0f,  0.0f,		// bottom left
		0.0f,  1.0f			// top left 
	};
}

void ScreenQuadMesh::initIndices(std::vector<int> &indices)
{
	indices = {
		0, 1, 3,
		1, 2, 3
	};
}
