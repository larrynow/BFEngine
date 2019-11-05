#pragma once
#ifndef BFDEVICERENDERER
#define BFDEVICERENDERER

#include"BFRenderPipeline.h"
#include<unordered_map>
#include<queue>
#include<functional>

/*
* The device renderer interface, implemented in terms of IBFRenderPipeline.
* Sharing buffers with IBFRenderPipeline.
*/

class IBFDeviceRenderer : private IBFRenderPipeline
{
public:

	IBFDeviceRenderer() {};

	virtual ~IBFDeviceRenderer() {};

	virtual void Init(UINT bufferWidth, UINT bufferHeight, IBFCamera* camera);

	void Clear(BFBitFiled mask = BF_CLEAR_ALL);

	inline void SetClearColor(COLOR3 clearColor = COLOR3(0.7f, 0.7f, 0.7f)) { mClearColor = clearColor; };

	/////////////////////////////////////////////////////
	// Use BFRenderPipeline to render.

	void RenderMesh(class Mesh* mesh);

	//void RenderPoints();

	/////////////////////////////////////////////////////
	// Directly buffer drawing function.

	void DrawLine(const COLOR3& color, UINT x1, UINT y1, UINT x2, UINT y2);

	void DrawRect(const COLOR3& color, UINT x1, UINT y1, UINT x2, UINT y2);

	void DrawTriangle(const COLOR3& color, const VEC2& v1_pixel, const VEC2& v2_pixel, const VEC2& v3_pixel);// Draw triangle

	virtual void Display() = 0;// Display on screen.

protected:

	/////////////////////////////////////////////////////
	// Color buffer settings shared with BFRenderPipeline.

	UINT mBufferWidth;
	UINT mBufferHeight;
	UINT mBufferSize;

	std::vector<float>* m_pZBuffer;
	std::vector<COLOR3>* m_pColorBuffer;
	COLOR3 mClearColor;// Color for clear.

	unsigned char* m_pFrameBuffer;

	void mFunction_UpdateFrameBufer();// Update framebuffer with colorBuffer.

	IBFCamera* m_pCamera;// A camera for rendering.

private:

	////////////////////////////////////////////////////
	// Helper functions.

	inline void mFunction_SetPixel(UINT x, UINT y, const COLOR3& color)
	{
		if (x < mBufferWidth && y < mBufferHeight)
		{
			m_pColorBuffer->at(y*mBufferWidth + x) = color;
		}
	};// Set m_pColorBuffer values.
};


#endif