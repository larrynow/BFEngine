#pragma once
#ifndef BFTYPES
#define BFTYPES

#include"BFMath.h"

#include<vector>

using namespace BFMath;

//////////////////////////////////////////
// Base types.

typedef unsigned int BFBitFiled;
#define BF_CLEAR_COLOR 0x001
#define BF_CLEAR_DEPTH 0x002
#define BF_CLEAR_ALL 0x00F

//////////////////////////////////////////
// Types for rendering.

typedef VEC3 COLOR3;
typedef VEC4 COLOR4;

// A vertex unit for drawing operation.
struct Vertex
{
	VEC3 Pos;
	COLOR4 Color;
	VEC3 Normal;
	VEC2 Texcoord;
};

// Vertex unit after Vertex Shader processing.
struct Vertex_VSO
{
	VEC4 ClipPos;// Positiobn in clip space.
	VEC4 Color;
	VEC2 Texcoord;
	//float ViewZ;// A view space z value to be used in interplotation.// USELESS.
	// Normal only need in lighting pocess.
};

struct Fragment
{
	UINT PixelX;
	UINT PixelY;
	VEC4 Color;
	VEC2 Texcoord;
};

struct RenderBuffer
{
	RenderBuffer(UINT _BufferWidth, UINT _BufferHeight, std::vector<float>* _pZBuffer, std::vector<VEC3>* _pColorBuffer):
		BufferWidth(_BufferWidth), BufferHeight(_BufferHeight),pZBuffer(_pZBuffer), pColorBuffer(_pColorBuffer) {}

	UINT BufferWidth;
	UINT BufferHeight;

	std::vector<float>* pZBuffer;
	std::vector<COLOR3>* pColorBuffer;
	//TODO : StencilBuffer.
};

struct RenderData
{
	RenderData() : pIndexBuffer(nullptr), pVertexBuffer(nullptr), Offset(0), VertexCount(0) {};
	RenderData(std::vector<Vertex>* _pVB, std::vector<UINT>* _pIB, UINT _offset, UINT _vCount) : 
		pVertexBuffer(_pVB), pIndexBuffer(_pIB), Offset(_offset), VertexCount(_vCount) {};
	std::vector<Vertex>* pVertexBuffer;
	std::vector<UINT>* pIndexBuffer;
	UINT Offset;// A offset for the buffers.
	UINT VertexCount;
};

struct Light
{
	COLOR3 AmbientColor;
	COLOR3 DiffuseColor;
	COLOR3 SpecluarColor;
	
	float DiffuseIntensity;
	float SpecularIntensity;
};

struct DirectionLight : public Light
{
	VEC3 Direction;
};

struct Material
{

};

struct Texture
{
	Texture() : Width(0), Height(0) { pColorBuffer = new std::vector<COLOR3>; };
	~Texture() { delete pColorBuffer; };

	COLOR3 GetPixel(UINT x, UINT y) const
	{
		if (x < Width && y < Height)
		{
			UINT index = y * Width + x;
			return pColorBuffer->at(index);

		}
		else
		return COLOR3();

	}

	std::vector<COLOR3>* pColorBuffer;
	UINT Width, Height;
};

class Mesh;

enum class BFDevice_t
{
	WIN_32
};


enum class BFInput 
{

	// Keyboard inputs.
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L

};

#endif // !1
