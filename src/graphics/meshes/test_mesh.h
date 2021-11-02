#pragma once

#include "meshes.h"

class TestMesh : public Mesh
{
private:
	void initPositions(std::vector<float> &pos);
	void initNormals(std::vector<float> &normals);
	void initTexCoords(std::vector<float> &texCoords);
	void initIndices(std::vector<int> &indices);
};