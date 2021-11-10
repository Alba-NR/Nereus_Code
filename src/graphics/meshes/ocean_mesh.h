#ifndef OCEAN_MESH
#define OCEAN_MESH
#pragma once

#include "meshes.h"

class OceanMesh : public Mesh
{
private:
	int m_grid_width;
	int m_grid_height;

	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);

public:
	OceanMesh(int grid_width, int grid_height);

	void updateMeshGrid(int new_grid_width, int new_grid_height);

	int getGridWidth() const;
	int getGridHeight() const;
};

#endif
