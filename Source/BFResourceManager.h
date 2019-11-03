#pragma once

#include"BFEngine.h"

// OBJ

struct OBJVertexIndex
{
	OBJVertexIndex(UINT _vi, UINT _ti, UINT _ni) : vertexID(_vi), texCoordID(_ti), vertexNormalID(_ni) {};
	OBJVertexIndex() : vertexID(0), texCoordID(0), vertexNormalID(0) {};

	inline bool operator ==(const OBJVertexIndex& _OV)const
	{
		return (vertexID == _OV.vertexID && texCoordID == _OV.texCoordID && vertexNormalID == _OV.vertexNormalID);
	}

	inline OBJVertexIndex& operator--()
	{
		--vertexID; --texCoordID; --vertexNormalID;
		return *this;
	}

	UINT vertexID;
	UINT texCoordID;
	UINT vertexNormalID;
};

// BMP

 //Already Defined in windows.h.
//struct BITMAPFILEHEADER {
//	WORD    bfType;
//	DWORD   bfSize;
//	WORD    bfReserved1;
//	WORD    bfReserved2;
//	DWORD   bfOffBits;
//};
//struct BITMAPINFOHEADER {
//	DWORD      biSize;
//	LONG       biWidth;
//	LONG       biHeight;
//	WORD       biPlanes;
//	WORD       biBitCount;
//	DWORD      biCompression;
//	DWORD      biSizeImage;
//	LONG       biXPelsPerMeter;
//	LONG       biYPelsPerMeter;
//	DWORD      biClrUsed;
//	DWORD      biClrImportant;
//};

class BFResourceManager
{
public:
	BFResourceManager();
	~BFResourceManager();

	bool ImportFile_OBJ(const std::string& filePath, std::vector<Vertex>& rVertexBuffer, std::vector<UINT>& rIndexBuffer);
	inline bool ImportMesh_OBJ(const std::string& filePath, Mesh* mesh) { return ImportFile_OBJ(filePath, *mesh->m_pVertexBuffer, *mesh->m_pIndexBuffer); };

	bool ImportFile_PPM(const std::string& filePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);
	inline bool ImportTexture_PPM(const std::string& filePath, Texture* tex) { return ImportFile_PPM(filePath, tex->Width, tex->Height, *tex->pColorBuffer); };

	bool ImportFile_BMP(const std::string& filePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);
	inline bool ImportTexture_BMP(const std::string& filePath, Texture* tex) { return ImportFile_BMP(filePath, tex->Width, tex->Height, *tex->pColorBuffer); };

	//bool ImportFile_PNG(const std::string& filePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);

private:

	//std::vector<Vertex>* m_pVB;
	//std::vector<UINT>* m_pIB;
};

