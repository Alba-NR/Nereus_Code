
#include "ocean_mesh.h"
#include <iostream>

OceanMesh::OceanMesh(unsigned int grid_width, unsigned int grid_height)
	: m_grid_width(grid_width), m_grid_height(grid_height)
{
}

void OceanMesh::initPositions(std::vector<float> &pos)
{
	for (int j = 0; j < m_grid_height + 1; j++)
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
	for (int j = 0; j < m_grid_height + 1; j++)
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
	for (int j = 0; j < m_grid_height + 1; j++)
	{
		for (int i = 0; i < m_grid_width + 1; i++)
		{
			texCoords.push_back(i / m_grid_width);  // u
			texCoords.push_back(j / m_grid_height); // v
		}
	}
}

void OceanMesh::initIndices(std::vector<int> &indices)
{
	int stride_h = (m_grid_width + 1);

	for (int j = 0; j < m_grid_height; j++)
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