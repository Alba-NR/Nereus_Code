#ifndef OCEAN_MESH
#define OCEAN_MESH
#pragma once

#include "meshes.h"

class OceanMesh : public Mesh
{
private:
	unsigned int m_grid_width;
	unsigned int m_grid_height;

	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);

public:
	OceanMesh(unsigned int grid_width, unsigned int grid_height);
};

#endif
