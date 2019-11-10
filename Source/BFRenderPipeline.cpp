#include "BFRenderPipeline.h"
#include"BFContent.h"
#include"BFThreadPool.h"

bool IBFRenderPipeline::Init(RenderBuffer & initBuffers)
{
	mBufferWidth = initBuffers.BufferWidth;
	mBufferHeight = initBuffers.BufferHeight;
	if (initBuffers.pColorBuffer != nullptr)
	{
		m_pColorBuffer = initBuffers.pColorBuffer;
	}
	else
	{
		MSG("Render pipeline initialization failed with INVALID color buffer.");
		return false;
	}

	if (initBuffers.pZBuffer != nullptr)
	{
		m_pZBuffer = initBuffers.pZBuffer;
	}
	else
	{
		MSG("Render pipeline initialization failed with INVALID Z buffer.");
		return false;
	}

	m_pVB_ClipSpace = new std::vector<Vertex_VSO>;
	m_pVB_ClipSpace_Clipped = new std::vector<Vertex_VSO>;
	m_pIB_ClipSpace_Clipped = new std::vector <UINT>;
	m_pFB_Rasterized = new std::unordered_map<UINT, Fragment>;

	return true;
}

void IBFRenderPipeline::RenderTriangles(RenderData & renderData)
{
	// Clear.
	m_pVB_ClipSpace->clear();
	m_pVB_ClipSpace_Clipped->clear();
	m_pIB_ClipSpace_Clipped->clear();
	m_pFB_Rasterized->clear();

	UINT offset = renderData.Offset;
	UINT vertexCount = renderData.VertexCount;
	auto const pVertexBuffer = renderData.pVertexBuffer;
	auto const pIndexBuffer = renderData.pIndexBuffer;

	m_pVB_ClipSpace->reserve(vertexCount);// If space is not enough.

	//////////////////////////////////////////////////
	// Vertex shader.
	auto t1 = timeGetTime();
	auto VSRenderSize = offset + vertexCount > pVertexBuffer->size() ? pVertexBuffer->size() : offset + vertexCount;
	for (size_t i = offset; i < VSRenderSize; ++i)
	{
		auto& currVertex = renderData.pVertexBuffer->at(offset + i);

		VertexShader(currVertex);
	}
	auto t2 = timeGetTime();
	/////////////////////////////////////////////////
	// Clipping.

	Clip_Triangles(pIndexBuffer);
	auto t3 = timeGetTime();

	if (m_pIB_ClipSpace_Clipped->empty())
		return;
	struct IndexUnit
	{
		UINT index1;
		UINT index2;
		UINT index3;
	};
	// Sort triangles by their z-values for overlap removal.
	std::vector<IndexUnit>* pIndexUintBuffers = (std::vector<IndexUnit> * )m_pIB_ClipSpace_Clipped;
	std::sort(pIndexUintBuffers->begin(), pIndexUintBuffers->end(),
		[this](const IndexUnit& IU1, const IndexUnit& IU2)
		{return max(m_pVB_ClipSpace_Clipped->at(IU1.index1).ClipPos.z, m_pVB_ClipSpace_Clipped->at(IU1.index2).ClipPos.z,
			m_pVB_ClipSpace_Clipped->at(IU1.index3).ClipPos.z) <
		max(m_pVB_ClipSpace_Clipped->at(IU2.index1).ClipPos.z, m_pVB_ClipSpace_Clipped->at(IU2.index2).ClipPos.z,
			m_pVB_ClipSpace_Clipped->at(IU2.index3).ClipPos.z); });


	////////////////////////////////////////////////
	// Rasterizing.

	RasterizeTriangles();

	auto t4 = timeGetTime();
	////////////////////////////////////////////////
	// Fragment Shader.

	BFThreadPool thread_pool(8);// Use thread pool to speed up.
	thread_pool.Clear();

	// Collect all tasks.
	for (auto it = m_pFB_Rasterized->begin(); it != m_pFB_Rasterized->end(); ++it)
	{
		auto& fragment = it->second;
		BFThreadPool::tasks.emplace_back([&fragment, this]() {FragmentShader(fragment); });// Faster than two para func.
	}
	auto t5 = timeGetTime();

	thread_pool.StartUp();// Start all threads then wait them all finish.

	auto t6 = timeGetTime();

	//std::cout << "vs : " << t2 - t1 << ", clip : " << t3 - t2 << ", Rasterize : " << t4 - t3 << ", FS : " << t5 - t4 << " : "<< t6 - t5 <<std::endl;

}

void IBFRenderPipeline::VertexShader(const Vertex & vertex)
{
	Vertex_VSO outVertex;

	// Clip space position.
	VEC4 pos(vertex.Pos, 1.0f);// w = 1.0f.
	pos = mWorldMatrix * pos;

	outVertex.WordPos = pos.xyz();// Save world position for fragment shader.

	pos = mViewMatrix * pos;
	pos = mProjMatrix * pos;

	// Clip space to NDC.
	pos.x /= pos.w;
	pos.y /= pos.w;
	pos.z /= pos.w;

	// Normal.
	MAT4 WorldMatrix_NoTranform = mWorldMatrix;
	WorldMatrix_NoTranform.SetColumn(3, { 0, 0, 0, 1.0f });// Just rotate, without transform.
	VEC4 norm_World = WorldMatrix_NoTranform * VEC4(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z, 1.0f);// Only when transform in Orthogonal way.

	outVertex.ClipPos = pos;
	outVertex.Color = vertex.Color;
	outVertex.Texcoord = vertex.Texcoord;
	outVertex.Normal = norm_World.xyz();

	m_pVB_ClipSpace->push_back(outVertex);
}

void IBFRenderPipeline::Clip_Triangles(std::vector<UINT>* const pIB)
{
	*m_pIB_ClipSpace_Clipped = (*pIB);// Pass in param will delete when Clip finish.
	*m_pVB_ClipSpace_Clipped = std::move(*m_pVB_ClipSpace);// Pass vertex buffers.

	UINT i = 0;
	while (i< m_pIB_ClipSpace_Clipped->size()-2)
	{
		if (m_pIB_ClipSpace_Clipped->empty())// All clip.
			return;
		UINT id1 = m_pIB_ClipSpace_Clipped->at(i);
		UINT id2 = m_pIB_ClipSpace_Clipped->at(i+1);
		UINT id3 = m_pIB_ClipSpace_Clipped->at(i+2);

		auto const v1 = m_pVB_ClipSpace_Clipped->at(id1);
		auto const v2 = m_pVB_ClipSpace_Clipped->at(id2);
		auto const v3 = m_pVB_ClipSpace_Clipped->at(id3);

		// For clip, using ndc coord in fact.
		auto OutOfBoundary = [](const Vertex_VSO& vertex) {return (vertex.ClipPos.x <= -1.0f || vertex.ClipPos.x >= 1.0f ||
			vertex.ClipPos.y <= -1.0f || vertex.ClipPos.y >= 1.0f ||
			vertex.ClipPos.z <= 0.0f || vertex.ClipPos.z >= 1.0f); };

		// Clip indices when three vertices is out of boundary.
		if (OutOfBoundary(v1) && OutOfBoundary(v2) && OutOfBoundary(v3))
		{
			// Clip by swap to tail then pop_back;
			auto clipTailIndex = m_pIB_ClipSpace_Clipped->size() - 3;
			std::swap(m_pIB_ClipSpace_Clipped->at(i), m_pIB_ClipSpace_Clipped->at(clipTailIndex));
			std::swap(m_pIB_ClipSpace_Clipped->at(i+1), m_pIB_ClipSpace_Clipped->at(clipTailIndex+1));
			std::swap(m_pIB_ClipSpace_Clipped->at(i+2), m_pIB_ClipSpace_Clipped->at(clipTailIndex+2));

			for (size_t i = 0; i < 3; i++)
			{
				m_pIB_ClipSpace_Clipped->pop_back();// Pop out the swapped indices.
			}
		}
		else
		{
			i += 3;// Do not clip.
		}
	}
}

void IBFRenderPipeline::RasterizeTriangles()
{
	int count = 0;
	if (m_pIB_ClipSpace_Clipped->empty())
		return;
	for (size_t i = 0; i < m_pIB_ClipSpace_Clipped->size()-2; i+=3)
	{

		auto id1 = m_pIB_ClipSpace_Clipped->at(i);
		auto id2 = m_pIB_ClipSpace_Clipped->at(i+1);
		auto id3 = m_pIB_ClipSpace_Clipped->at(i+2);

		const auto& v1 = m_pVB_ClipSpace_Clipped->at(id1);
		const auto& v2 = m_pVB_ClipSpace_Clipped->at(id2);
		const auto& v3 = m_pVB_ClipSpace_Clipped->at(id3);

		// From (-1.0, 1.0) to screen space.
		auto convertToPixelSpace = [this](const Vertex_VSO& vertex, VEC2& pixelCoord) { pixelCoord.x = mBufferWidth * (vertex.ClipPos.x + 1.0f) / 2.0f; 
			pixelCoord.y = mBufferHeight * (vertex.ClipPos.y + 1.0f) / 2.0f; };

		VEC2 pixelCoord1, pixelCoord2, pixelCoord3;
		convertToPixelSpace(v1, pixelCoord1);
		convertToPixelSpace(v2, pixelCoord2);
		convertToPixelSpace(v3, pixelCoord3);

		// Two base vector from v1 to v2 and v1 to v3.
		VEC2 Vec1_2 = pixelCoord2 - pixelCoord1;
		VEC2 Vec1_3 = pixelCoord3 - pixelCoord1;

		// The Z coordinate value of cross product of two vectors.
		float d_Z = Vec1_2.x*Vec1_3.y - Vec1_2.y*Vec1_3.x;
		if (d_Z > 0)
		{
			// Back face.
			continue;
		}
		else if (d_Z<0)
		{
			// Front face.
			//continue;
		}
		else
		{
			//continue;// Three points in one line.
		}

		// Scanline Y pixel coordinate value range.
		float minY = Clamp(min(min(pixelCoord1.y, pixelCoord2.y), pixelCoord3.y), 0, mBufferHeight - 1);
		float maxY = Clamp(max(max(pixelCoord1.y, pixelCoord2.y), pixelCoord3.y), 0, mBufferHeight - 1);

		// Horizontal line intersect for each Y.
		for (size_t y = UINT(minY); y < UINT(maxY)+1; ++y)
		{
			// Horizontal intersection.
			UINT x1, x2;
			if (mFunction_HorizontalIntersect(float(y), pixelCoord1, pixelCoord2, pixelCoord3, x1, x2))
			{
				// In x direction.
				for (size_t x = x1; x <= x2; ++x)
				{
					if (x >= mBufferWidth || y>= mBufferHeight)// Out of bundary, (Clip only when 3 vertices all out of bundary.)
					{
						break;
					}

					// Bilinear interpolation for vertex coordinates.
					VEC2 currentPoint_Coord(float(x) + 0.f, float(y) + 0.f);

					VEC2 VecV1_currentPoint = currentPoint_Coord - pixelCoord1;// From v1 to current point.

					// Calculate interpolation ratio by cross product;
					float s = (VecV1_currentPoint.x * Vec1_3.y - VecV1_currentPoint.y * Vec1_3.x) / d_Z;// The portion ratio of v2 color: area(V1, v3, current)/area(v1, v3, v2).
					float t = (Vec1_2.x * VecV1_currentPoint.y - Vec1_2.y * VecV1_currentPoint.x) / d_Z;// The portion ratio of V3 color.

					// Get current 1/z for interpolate values.
					float one_div_z = (s / v2.ClipPos.w + t / v3.ClipPos.w + (1 - t - s) / v1.ClipPos.w);// w is z in view.

					//float depth = (s / v2.ClipPos.w * v2.ClipPos.z +
					//	t / v3.ClipPos.w * v3.ClipPos.z +
					//	(1 - t - s) / v1.ClipPos.w * v1.ClipPos.z)/one_div_z;
					float depth = s * v2.ClipPos.z + t * v3.ClipPos.z + (1 - t - s) * v1.ClipPos.z;
					//PRINT(depth);
					// Depth Test.
					if (DepthTest(x, y, depth))
					{
						//m_pZBuffer->at(y * mBufferWidth + x) = depth;
						// if(ZTEST_writtable)...

						Fragment outFrag;

						outFrag.PixelX = x;
						outFrag.PixelY = y;

						// Calculate color by perspective interpolation.
						outFrag.Color = (s / v2.ClipPos.w * v2.Color +
							t / v3.ClipPos.w * v3.Color +
							(1 - t - s) / v1.ClipPos.w * v1.Color) / one_div_z;

						// Calculate Texcoord by perspective interpolation.
						outFrag.Texcoord = (s * v2.Texcoord / v2.ClipPos.w +
							t * v3.Texcoord / v3.ClipPos.w +
							(1 - t - s) * v1.Texcoord / v1.ClipPos.w) / one_div_z;

						outFrag.FragPos = (s * v2.WordPos / v2.ClipPos.w +
							t * v3.WordPos / v3.ClipPos.w +
							(1 - t - s) * v1.WordPos / v1.ClipPos.w) / one_div_z;

						outFrag.Normal = (s * v2.Normal / v2.ClipPos.w +
							t * v3.Normal / v3.ClipPos.w +
							(1 - t - s) * v1.Normal / v1.ClipPos.w) / one_div_z;

						(*m_pFB_Rasterized)[y * mBufferWidth + x] = outFrag;
					}
					else
						count++;
				}
			}

		}
	}// For every clipped vertex.
	PRINT(count);
}

void IBFRenderPipeline::RasterizePoints()
{
	for (size_t i = 0; i < m_pIB_ClipSpace_Clipped->size(); i++)
	{
		Fragment outFragment;

		auto id = m_pIB_ClipSpace_Clipped->at(i);
		const auto& vertex = m_pVB_ClipSpace_Clipped->at(i);

		VEC2 pixelCoord(mBufferWidth * (vertex.ClipPos.x + 1.0f) / 2.0f, mBufferHeight * (vertex.ClipPos.y + 1.0f) / 2.0f);

		// Depth test.
		float depth = vertex.ClipPos.z;
		/*if (!DepthTest(pixelCoord.x, pixelCoord.y, depth))
			continue;*/

		outFragment.PixelX = UINT(pixelCoord.x);
		outFragment.PixelY = UINT(pixelCoord.y);

		// Write to depth Test.
		//WriteDepthBuffer(outFragment.PixelX, outFragment.PixelY, depth);

		outFragment.Color = vertex.Color;
		outFragment.Texcoord = vertex.Texcoord;

		//m_pFB_Rasterized->push_back(outFragment);
	}


}

bool IBFRenderPipeline::DepthTest(UINT pixel_x, UINT pixel_y, float depth)
{
	if (depth < (m_pZBuffer->at(pixel_y*mBufferWidth + pixel_x)))
	{
		//PRINT(depth << ":" << (m_pZBuffer->at(pixel_y * mBufferWidth + pixel_x)));
		m_pZBuffer->at(pixel_y * mBufferWidth + pixel_x) = depth;
		return true;
	}
	else
		return false;
}

void IBFRenderPipeline::FragmentShader(Fragment& inFrag)
{
	COLOR3 outColor;
	COLOR3 texSampleColor = mFunction_SampleTexture(inFrag.Texcoord.x, inFrag.Texcoord.y);
	outColor = inFrag.Color.xyz() * texSampleColor;

	// For lights.
	auto viewDir = inFrag.FragPos - mCameraPos;
	for (auto pLight : BFContent::m_pLights)
	{
		if (pLight == nullptr) break;
		if (DirectionLight* pDLight = dynamic_cast<DirectionLight*>(pLight))
		{
			auto lightDir = pDLight->Direction;
			float diff = max(inFrag.Normal.CosineValue(-lightDir), 0.f);
			auto reflectDir = Reflect(lightDir, inFrag.Normal);
			float spec = (-viewDir).CosineValue(reflectDir);

			COLOR3 ambient = pDLight->AmbientColor * texSampleColor;
			COLOR3 diffuse = pDLight->DiffuseColor * diff *texSampleColor;// Should multiply by a material intensity.
			COLOR3 specular = pDLight->SpecluarColor * spec * texSampleColor;

			outColor += (ambient + diffuse + specular);
			//std::cout << outColor << std::endl;
		}
		else if(PointLight* pPLight = dynamic_cast<PointLight*>(pLight))
		{
			auto lightDir = inFrag.FragPos - pPLight->Position;
			float diff = max(inFrag.Normal.CosineValue(-lightDir), 0.f);
			auto reflectDir = Reflect(lightDir, inFrag.Normal);
			float spec = (-viewDir).CosineValue(reflectDir);

			COLOR3 ambient = pPLight->AmbientColor * texSampleColor;
			COLOR3 diffuse = pPLight->DiffuseColor * diff * texSampleColor;// Should multiply by a material intensity.
			COLOR3 specular = pPLight->SpecluarColor * spec * texSampleColor;

			// Attenuation.
			float distance = (inFrag.FragPos - pPLight->Position).Length();
			float attenuation = 1.f / (pPLight->Constant + pPLight->Linear * distance
				+ pPLight->Quadratic * distance * distance);
			ambient *= attenuation;
			diffuse *= attenuation;
			specular *= attenuation;

			outColor += (ambient + diffuse + specular);
		}
		else if (SpotLight* pSLight = dynamic_cast<SpotLight*>(pLight))
		{
			auto lightDir = inFrag.FragPos - pSLight->Position;
			float theta = lightDir.CosineValue(pSLight->Direction);
			float intensity = Clamp((theta - pSLight->OutterTheta) / pSLight->Epsilon, 0.f, 1.f);

			float diff = max(inFrag.Normal.CosineValue(-lightDir), 0.f);
			auto reflectDir = Reflect(lightDir, inFrag.Normal);
			float spec = (-viewDir).CosineValue(reflectDir);

			COLOR3 ambient = pSLight->AmbientColor * texSampleColor;
			COLOR3 diffuse = pSLight->DiffuseColor * diff * texSampleColor;// Should multiply by a material intensity.
			COLOR3 specular = pSLight->SpecluarColor * spec * texSampleColor;

			outColor += (ambient + diffuse * intensity + specular * intensity);
			
		}
	}

	ClampColor(outColor);
	m_pColorBuffer->at(inFrag.PixelY * mBufferWidth + inFrag.PixelX) = outColor;
	
}

bool IBFRenderPipeline::mFunction_HorizontalIntersect(float y, const VEC2 & v1, const VEC2 & v2, const VEC2 & v3, UINT & outX1, UINT & outX2)
{
	const VEC2 triangleVertices[3] = { v1, v2, v3 };

	// Calculate how many vertices have a Y coord with y.
	std::vector<UINT> indicesWithY;
	if (v1.y == y) indicesWithY.push_back(0);
	if (v2.y == y) indicesWithY.push_back(1);
	if (v3.y == y) indicesWithY.push_back(2);
	
	auto intersectAtStrip = [](float y, const VEC2& v1, const VEC2& v2, VEC2& outItersectedPoint)->bool
	{
		float minY = min(v1.y, v2.y);
		float maxY = max(v1.y, v2.y);
		if (y<minY || y>maxY) return false;// Can't intersect.
		else
		{
			outItersectedPoint = Lerp(v1, v2, (y - v1.y) / (v2.y - v1.y));
			return true;
		}
	};

	switch (indicesWithY.size())
	{
		// Case 0 with no vertex have a y vakue in Y coord.
		case 0:
		{
			// Find intersections at all three line_strips.
			std::vector<VEC2> intersectPoints;
			VEC2 point;
			if (intersectAtStrip(y, v1, v2, point)) intersectPoints.push_back(point);
			if (intersectAtStrip(y, v1, v3, point)) intersectPoints.push_back(point);
			if (intersectAtStrip(y, v2, v3, point)) intersectPoints.push_back(point);

			if (intersectPoints.size() == 2)
			{
				outX1 = UINT(Clamp(intersectPoints.at(0).x, 0, float(mBufferWidth)));
				outX2 = UINT(Clamp(intersectPoints.at(1).x, 0, float(mBufferWidth)));

				// Let return x1 < x2.
				if (outX1 > outX2) std::swap(outX1, outX2);

				return true;
			}
			else
			{
				return false;
			}
			break;
		}
		case 1:
		{
			// One point have a Y value equls y.
			VEC2 point;
			bool haveIntersection = false;
			switch (indicesWithY.at(0))// See which strip use to intersect with.
			{
				case 0://v1 have a Y value y, looking for intersection on line_2_3.
				{
					haveIntersection = intersectAtStrip(y, v2, v3, point);
					break;
				}
				case 1://v2 have a Y value y, looking for intersection on line_1_3.
				{
					haveIntersection = intersectAtStrip(y, v1, v3, point);
					break;
				}
				case 2://v3 have a Y value y, looking for intersection on line_2_3.
				{
					haveIntersection = intersectAtStrip(y, v1, v2, point);
					break;
				}
				default:
					break;
			}
			if (haveIntersection)
			{
				outX1 = UINT(Clamp(point.x, 0, float(mBufferWidth)));
				outX2 = UINT(Clamp(triangleVertices[indicesWithY.at(0)].x, 0, float(mBufferWidth)));// The y value point's x coord.
				if (outX1 > outX2) std::swap(outX1, outX2);
				return true;
			}
			else// No intersection.
			{
				return false;
			}

			break;
		}
		case 2:
		{
			// Two points have a y value in Y.
			outX1 = UINT(Clamp(triangleVertices[indicesWithY.at(0)].x, 0, float(mBufferWidth)));
			outX2 = UINT(Clamp(triangleVertices[indicesWithY.at(1)].x, 0, float(mBufferWidth)));
			if (outX1 > outX2) std::swap(outX1, outX2);
			return true;
			break;
		}
	default:
		return false;
		break;
	}
}

inline VEC3 IBFRenderPipeline::mFunction_SampleTexture(float x, float y) const
{
	// Mapping texture color to frag.
	if (!m_pTexture) return VEC3(1.0f);

	UINT pixelX, pixelY;

	pixelX = (UINT)abs((m_pTexture->Width - 1) * (x - UINT(x)));
	pixelY = (UINT)abs((m_pTexture->Height - 1) * (y - UINT(y)));

	return m_pTexture->GetPixel(pixelX, pixelY);
}
