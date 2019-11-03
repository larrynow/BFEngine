#include "BFGeometry.h"



void BFGeometry::CreateCube(const float width, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	////////////////////////////////////////////////////////////
	// Build 6 faces, each face with 2 triangles(6 vertices) all in anticlockwise.

	vertices.clear();
	indices.clear();

	// Front face.
	vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 0.f) });// Bottom-left.
	vertices.push_back({ VEC3( width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 1.f) });// Top-right.
	vertices.push_back({ VEC3( width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 0.f) });// Bottom-right.
	vertices.push_back({ VEC3( width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 1.f) });// Top-right.
	vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 0.f) });// Bottom-left.
	vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 1.f) });// Top-left.

	// Back face.
	vertices.push_back({ VEC3(-width/2, -width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 0.f) });
	vertices.push_back({ VEC3( width/2, -width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3( width/2,  width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3( width/2,  width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(-width/2,  width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width/2, -width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 0.f) });

	// Left face.
	vertices.push_back({ VEC3(-width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 0.f) });
	vertices.push_back({ VEC3(-width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 0.f) });

	// Right face.
	vertices.push_back({ VEC3(width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 0.f) });

	// Bottom face.
	vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3( width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3( width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3( width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 0.f) });
	vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 1.f) });

	// Up face.
	vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3( width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3( width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3( width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 0.f) });

	for (size_t i = 0; i < 36; i++)
	{
		indices.push_back(i);
	}
}
