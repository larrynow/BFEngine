#pragma once
#ifndef BFRENDERPIPELINE
#define BFRENDERPIPELINE

#include "BFEngine.h"

/*
* A render pipeline interface.
*/

class IBFRenderPipeline
{
public:

	// Render pipeline initializer, init with buffers from device renderer.
	bool Init(RenderBuffer& initBuffers);

protected:

	////////////////////////////////////////////
	// Rendering functions call by Device renderer.

	void RenderTriangles(RenderData& renderData);

	void RenderPoints(RenderData& renderData);

public:

	////////////////////////////////////////////
	// Matrixes settings for render.

	inline void SetWorldMatrix(const MAT4& world) { mWorldMatrix = world; };

	inline void SetViewMatrix(const MAT4& view) { mViewMatrix = view; };

	inline void SetProjectionMatrix(const MAT4& projection) { mProjMatrix = projection; };

	inline void SetCameraPos(const VEC3& pos) { mCameraPos = pos; };

	////////////////////////////////////////////
	// Light settings for render.

	void AddLight(const Light& light);

	inline void EnableLight() { bLightingEnable = true; };

	inline void DisableLight() { bLightingEnable = false; };

	inline void EnableZTest() { bZTestEnable = true; };

	inline void DisableZTest() { bZTestEnable = false; };

	inline void SetMaterial(const Material& material) { mMaterial = material; };

	inline void SetTexture(Texture* _pTexture) { m_pTexture = _pTexture; };

private:

	/////////////////////////////////////////////
	// Buffer Variables shared from DeviceRenderer.

	UINT mBufferWidth;
	UINT mBufferHeight;
	std::vector<float>* m_pZBuffer;
	std::vector<COLOR3>* m_pColorBuffer;

	bool bZTestEnable;// ZTest flag, controlled by DeviceRender.

	/////////////////////////////////////////////
	// Render Variables.

	MAT4 mWorldMatrix;
	MAT4 mViewMatrix;
	MAT4 mProjMatrix;
	VEC3 mCameraPos;

	static const UINT MAXLIGHTNUM = 8;
	bool bLightingEnable;
	Light mLights[MAXLIGHTNUM];

	Material mMaterial;
	const Texture* m_pTexture;

	/////////////////////////////////////////////
	// Render Pipeline.

	void VertexShader(const Vertex& vertex);

	std::vector<Vertex_VSO>* m_pVB_ClipSpace;// vertices in Clip space.

	std::vector<Vertex_VSO>* m_pVB_ClipSpace_Clipped;// Clipped vertices in Clip space.

	std::vector <UINT>* m_pIB_ClipSpace_Clipped;// Clipped indeices in Clip space.

	void Clip_Triangles(std::vector<UINT>* const pIB);

	void RasterizeTriangles();

	void RasterizePoints();

	std::vector<Fragment>* m_pFB_Rasterized;

	bool DepthTest(UINT pixel_x, UINT pixel_y, float depth);

	void FragmentShader_DrawTriangles(Fragment& inFrag);

	bool mFunction_HorizontalIntersect(float y, const VEC2& v1, const VEC2& v2, const VEC2& v3, UINT& outX1, UINT& outX2);// Get two x coordinates at y.

	VEC3 mFunction_SampleTexture(float x, float y);
};

#endif // BFRENDER
