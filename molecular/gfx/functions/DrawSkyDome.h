/*	DrawSkyDome.h

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*/

#ifndef MOLECULAR_GFX_DRAWSKYDOME_H
#define MOLECULAR_GFX_DRAWSKYDOME_H

#include "DrawingFunction.h"
#include <molecular/util/Matrix4.h>
#include <molecular/util/Math.h>
#include <molecular/gfx/TextureManager.h>
#include <cstdint>

namespace molecular
{
namespace gfx
{
using namespace util;

/// Draw textures sky dome
/** Always writes 1.0 to Z so that it is drawn behind everything else. Use with 360deg by 90deg
	panoramic textures. */
template<class TRenderManager>
class DrawSkyDome : public DrawingFunction
{
public:
	using RenderManager = TRenderManager;

	DrawSkyDome(RenderManager& renderManager);
	~DrawSkyDome();

	void Execute() override;

	/// Returns infinite bounds, so the sky dome is always rendered
	util::AxisAlignedBox GetBounds() const override;

	void SetTexture(Hash texture);

private:
	RenderManager& mRenderManager;
	TextureManager::Asset* mTexture = nullptr;
	typename RenderManager::RenderCmdSink::VertexBuffer* mVertexBuffer = nullptr;
	typename RenderManager::RenderCmdSink::IndexBuffer* mIndexBuffer = nullptr;
	VertexAttributeInfo mPositionInfo;
	VertexAttributeInfo mUvInfo;
	IndexBufferInfo mIndexInfo;
};

template<class TRenderManager>
DrawSkyDome<TRenderManager>::DrawSkyDome(RenderManager& renderManager) :
    DrawingFunction(renderManager),
    mRenderManager(renderManager)
{

}

template<class TRenderManager>
DrawSkyDome<TRenderManager>::~DrawSkyDome()
{
	mRenderer.DestroyVertexBuffer(mVertexBuffer);
}

template<class TRenderManager>
void DrawSkyDome<TRenderManager>::Execute()
{
	if(!mVertexBuffer)
	{
		const unsigned int polarSteps = 8;
		const unsigned int azimuthSteps = 24;
		std::vector<float> vertexData;
		vertexData.reserve(polarSteps * azimuthSteps * 5);
		const float polarStep = 0.5f * Math::kPi_f / (polarSteps - 1);
		const float azimuthStep = 2.0f * Math::kPi_f / (azimuthSteps - 1);
		const float radius = 100;
		const float uStep = 1.0f / (azimuthSteps - 1);
		const float vStep = 1.0f / (polarSteps - 1);
		for(unsigned int p = 0; p < polarSteps; p++)
		{
			for(unsigned int a = 0; a < azimuthSteps; a++)
			{
				vertexData.push_back(radius * std::sin(p * polarStep) * std::cos(a * azimuthStep)); // X
				vertexData.push_back(radius * std::sin(p * polarStep) * std::sin(a * azimuthStep)); // Y
				vertexData.push_back(radius * std::cos(p * polarStep)); // Z
				vertexData.push_back(uStep * a); // U
				vertexData.push_back(vStep * p); // V
			}
		}
		mVertexBuffer = mRenderer.CreateVertexBuffer();
		mVertexBuffer->Store(vertexData.data(), 5 *  vertexData.size());
		mPositionInfo.components = 3;
		mPositionInfo.stride = 5 * sizeof(float);
		mPositionInfo.type = VertexAttributeInfo::kFloat;
		mUvInfo.components = 2;
		mUvInfo.offset = 3 * sizeof(float);
		mUvInfo.stride = 5 * sizeof(float);
		mUvInfo.type = VertexAttributeInfo::kFloat;

		// Indices:
		std::vector<uint16_t> indexData;
		indexData.reserve(polarSteps * azimuthSteps * 6);
		for(unsigned int p = 0; p < polarSteps - 1; p++)
		{
			for(unsigned int a = 0; a < azimuthSteps - 1; a++)
			{
				uint16_t p0 = p * azimuthSteps + a;
				uint16_t p1 = p * azimuthSteps + (a + 1);
				uint16_t p2 = (p + 1) * azimuthSteps + (a + 1);
				uint16_t p3 = (p + 1) * azimuthSteps + a;
				indexData.push_back(p0);
				indexData.push_back(p1);
				indexData.push_back(p2);
				indexData.push_back(p0);
				indexData.push_back(p2);
				indexData.push_back(p3);
			}
		}
		mIndexBuffer = mRenderer.CreateIndexBuffer();
		mIndexBuffer->Store(indexData.data(), indexData.size() * sizeof(uint16_t));
		mIndexInfo.count = indexData.size();
		mIndexInfo.type = IndexBufferInfo::Type::kUInt16;
	}

	Binding<Uniform<Matrix4>> viewMatrix("viewMatrix"_H, this);
	Binding<Uniform<Matrix4>> modelMatrix("modelMatrix"_H, this);
	**viewMatrix = Matrix4::Translation(-(**viewMatrix).GetTranslation()) * **viewMatrix;
	**modelMatrix = Matrix4::Identity();

	/* Choose different name for attribute to select program snippet writing 1.0 to Z. */
	Binding<Attribute> vertexPositionAttr("skyVertexPositionAttr"_H, this);
	Binding<Attribute> vertexUv0Attr("vertexUv0Attr"_H, this);
	Binding<Uniform<RenderCmdSink::Texture*>> diffuseTexture("diffuseTexture"_H, this);

	*vertexPositionAttr = Attribute(mVertexBuffer, mPositionInfo);
	*vertexUv0Attr = Attribute(mVertexBuffer, mUvInfo);
	if(mTexture)
		**diffuseTexture = mTexture->Use();
	mRenderer.SetDepthState(true, false, RenderCmdSink::kEqual);
	PrepareProgram();
	mRenderer.Draw(mIndexBuffer, mIndexInfo);
	mRenderer.SetDepthState(true, true);

}

template<class TRenderManager>
util::AxisAlignedBox DrawSkyDome<TRenderManager>::GetBounds() const
{
	// Return infinite box
	return util::AxisAlignedBox();
}

template<class TRenderManager>
void DrawSkyDome<TRenderManager>::SetTexture(Hash texture)
{
	mTexture = mRenderManager.GetTextureManager().GetAsset(texture);
}

}
}
#endif // MOLECULAR_GFX_DRAWSKYDOME_H
