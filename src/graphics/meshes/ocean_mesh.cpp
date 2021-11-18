
#define NOMINMAX // to disable 'max' macro from minwindef.h #included indirectly
#include "ocean_mesh.h"
#include "../../main/constants.h"


OceanMesh *OceanMesh::s_instance = nullptr;

OceanMesh &OceanMesh::getInstance()
{
	if (OceanMesh::s_instance == nullptr)
	{
		OceanMesh::s_instance = new OceanMesh(
			NereusConstants::DEFAULT_OCEAN_WIDTH,
			NereusConstants::DEFAULT_OCEAN_LENGTH
		);
		OceanMesh::s_instance->initialise();
	}

	return *OceanMesh::s_instance;
}

OceanMesh::OceanMesh(int grid_width, int grid_height)
	: m_grid_width(std::max(grid_width,1)), m_grid_length(std::max(grid_height,1))
{
}

void OceanMesh::initPositions(std::vector<float> &pos)
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

void OceanMesh::initNormals(std::vector<float> &normals)
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

void OceanMesh::initTexCoords(std::vector<float> &texCoords)
{
	for (int j = 0; j < m_grid_length + 1; j++)
	{
		for (int i = 0; i < m_grid_width + 1; i++)
		{
			texCoords.push_back(i / m_grid_width);  // u
			texCoords.push_back(j / m_grid_length); // v
		}
	}
}

void OceanMesh::initIndices(std::vector<int> &indices)
{
	int stride_h = (m_grid_width + 1);

	for (int j = 0; j < m_grid_length; j++)
	{
		for (int i = 0; i < m_grid_width; i++)
		{
			// bottom triangle
			indices.push_back(stride_h * j + i);
			indices.push_back(stride_h * j + i + 1);
			indices.push_back(stride_h * (j + 1) + i);
			
			// top triangle
			indices.push_back(stride_h * (j + 1) + i);
			indices.push_back(stride_h * j + i + 1);
			indices.push_back(stride_h * (j + 1) + i + 1);
		}
	}
}

// Updates the OpenGL buffer data to that for a grid with the new dimensions/resolution
void OceanMesh::updateMeshGrid(int new_grid_width, int new_grid_length)
{
	m_grid_width = new_grid_width;
	m_grid_length = new_grid_length;

	this->initialise();
}

int OceanMesh::getGridWidth() const
{
	return m_grid_width;
}

int OceanMesh::getGridLength() const
{
	return m_grid_length;
}