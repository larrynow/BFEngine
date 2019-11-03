#include"BFDeviceRenderer.h"

void IBFDeviceRenderer::Init(UINT bufferWidth, UINT bufferHeight)
{
	/* 
	* First initialize display devices,
	* include set output,
	* create a output buffer...
	* implemented in child class DeviceRendererPlatform. 
	*
	* Function_InitDisplayDevice();
	*/

	mBufferWidth = bufferWidth;
	mBufferHeight = bufferHeight;
	mBufferSize = bufferWidth * bufferHeight;

	m_pCamera = new IBFCamera();

	// Create buffers;
	m_pZBuffer = new std::vector<float>(mBufferSize);
	m_pColorBuffer = new std::vector<COLOR3>(mBufferSize, 1.f);

	// Initialize RnederPipeline.
	RenderBuffer renderBuffer(mBufferWidth, mBufferHeight, m_pZBuffer, m_pColorBuffer);
	IBFRenderPipeline::Init(renderBuffer);

}

void IBFDeviceRenderer::Clear(BFBitFiled mask)
{
	if (mask & BF_CLEAR_COLOR)
	{
		for (size_t i = 0; i < mBufferSize; i++)
		{
			m_pColorBuffer->at(i) = mClearColor;
		}
	}

	if (mask & BF_CLEAR_DEPTH)
	{
		for (size_t i = 0; i < mBufferSize; i++)
		{
			m_pZBuffer->at(i) = 1.0f;
		}
	}
}

void IBFDeviceRenderer::RenderMesh(Mesh* mesh)
{
	if (!m_pCamera) return;

	MAT4 worldMat, projMat ,viewMat;
	mesh->GetWorldMatrx(worldMat);
	m_pCamera->GetViewMatrix(viewMat);
	projMat = GetPerspectiveMatrix(GetRadian(m_pCamera->GetFOV()/2), float(mBufferWidth) / float(mBufferHeight), m_pCamera->GetNearPlane(), m_pCamera->GetFarPlane());

	IBFRenderPipeline::SetWorldMatrix(worldMat);
	IBFRenderPipeline::SetProjectionMatrix(projMat);
	IBFRenderPipeline::SetViewMatrix(viewMat);
	IBFRenderPipeline::SetCameraPos(m_pCamera->GetPos());

	IBFRenderPipeline::SetTexture(mesh->m_pTexture);

	RenderData renderData;
	renderData.Offset = 0;
	renderData.pIndexBuffer = mesh->m_pIndexBuffer;
	renderData.pVertexBuffer = mesh->m_pVertexBuffer;
	renderData.VertexCount = mesh->GetVertexCount();

	IBFRenderPipeline::RenderTriangles(renderData);
}

void IBFDeviceRenderer::DrawLine(const COLOR3 & color, UINT x1, UINT y1, UINT x2, UINT y2)
{
	// For vertical line with no slope.
	if (x1 == x2)
	{
		for (size_t _y = y1; _y <= y2; _y++)
		{
			mFunction_SetPixel(x1, _y, color);
		}
	}
	else
	{
		// Calculate slope.
		float k = float(int(y2 - y1)) / float(int(x2 - x1));// float have alarger range than UINT, loss some accuracy.
		float k_inv = 1 / k;

		// Bresenham
		float offset = 0.0f;
		if (abs(k) <= 1.0f)// y step is smaller than x step, set x step be 1, y step be k.
		{
			UINT x = x1;
			while (x!=x2)
			{
				mFunction_SetPixel(x, y1 + UINT(offset), color);
				offset += k;
				if (x2 > x1)++x;
				else --x;
			}
		}
		else// or set y step be 1, x step be 1/k(always make sure steps <= 1).
		{
			UINT y = y1;
			while (y!=y2)
			{
				mFunction_SetPixel(x1 + UINT(offset), y, color);
				offset += k_inv;
				if (y2 > y1)++y;
				else --y;
			}
		}
	}
	
}

void IBFDeviceRenderer::DrawRect(const COLOR3 & color, UINT x1, UINT y1, UINT x2, UINT y2)
{
	for (UINT i = x1; i <= x2; ++i)
		for (UINT j = y1; j <= y2; ++j)
			mFunction_SetPixel(i, j, color);
}

void IBFDeviceRenderer::DrawTriangle(const COLOR3 & color, const VEC2 & v1_pixel, const VEC2 & v2_pixel, const VEC2 & v3_pixel)
{
	// Draw a device visiable triangle, all coord is final coord in screen space.s
	MAT4 matIdentity;// Set all transform matrices as identity.
	IBFRenderPipeline::SetWorldMatrix(matIdentity);
	IBFRenderPipeline::SetViewMatrix(matIdentity);
	IBFRenderPipeline::SetProjectionMatrix(matIdentity);

	auto screen2Clip = [this](const VEC2& _pixel) {return VEC2(_pixel.x / mBufferWidth * 2.0f - 1.0f, _pixel.y / mBufferHeight * 2.0f - 1.0f); };

	std::vector<Vertex> vertexBufferArray(3);
	vertexBufferArray[0].Color = COLOR4(color, 1.0f);
	vertexBufferArray[0].Pos = VEC3(screen2Clip(v1_pixel), 0.5f);// DELETE : A very small depth, delete after projection mateix finishs.
	vertexBufferArray[0].Normal = VEC3(0.0f, 0.0f, 1.0f);

	vertexBufferArray[1].Color = COLOR4(color, 1.0f);
	vertexBufferArray[1].Pos = VEC3(screen2Clip(v2_pixel), 0.5f);
	vertexBufferArray[1].Normal = VEC3(0.0f, 0.0f, 1.0f);
	
	vertexBufferArray[2].Color = COLOR4(color, 1.0f);	
	vertexBufferArray[2].Pos = VEC3(screen2Clip(v3_pixel), 0.5f);
	vertexBufferArray[2].Normal = VEC3(0.0f, 0.0f, 1.0f);

	std::vector<UINT> indexBufferArray{0, 1, 2};

	RenderData renderData(&vertexBufferArray, &indexBufferArray, 0, 3);

	IBFRenderPipeline::RenderTriangles(renderData);
}

void IBFDeviceRenderer::mFunction_UpdateFrameBufer()
{
	memset(m_pFrameBuffer, 255, 800 * 600 * 3);// Set memory with 0.
	for (size_t i = 0; i < m_pColorBuffer->size(); ++i)
	{
		m_pFrameBuffer[3 * i] = UINT(m_pColorBuffer->at(i).z * 255);
		m_pFrameBuffer[3 * i + 1] = UINT(m_pColorBuffer->at(i).y * 255);
		m_pFrameBuffer[3 * i + 2] = UINT(m_pColorBuffer->at(i).x * 255);
	}
}
