#pragma once
#ifndef BFGEOMETRY
#define BFGEOMETRY
#include"BFengine.h"

class BFGeometry
{
public:

	static void CreateCube(const float width, std::vector<Vertex>& vertices, std::vector<UINT>& indices);// Create a Cube's vertices and indices.
	inline static void CreateCubeMesh(const float width, Mesh& mesh) { CreateCube(width, *mesh.m_pVertexBuffer, *mesh.m_pIndexBuffer);};


};

#endif // !BFGEOMETRY


