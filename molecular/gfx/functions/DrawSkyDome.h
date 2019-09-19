/*	DrawSkyDome.h
	Copyright 2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DRAWSKYDOME_H
#define DRAWSKYDOME_H

#include "DrawingFunction.h"
#include "util/Matrix4.h"
#include <cstdint>

namespace Gfx
{

template<class TRenderManager>
class DrawSkyDome : public DrawingFunction
{
public:
	using RenderManager = TRenderManager;

	DrawSkyDome(RenderManager& renderManager);
	~DrawSkyDome();

	void Execute() override;
	AxisAlignedBox GetBounds() const override;

	void SetTexture(Hash texture);

private:
	RenderManager& mRenderManager;
	typename RenderManager::TextureManager::Asset* mTexture = nullptr;
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
		const float polarStep = 0.5f * kPi_f / (polarSteps - 1);
		const float azimuthStep = 2.0f * kPi_f / (azimuthSteps - 1);
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
AxisAlignedBox DrawSkyDome<TRenderManager>::GetBounds() const
{
	return AxisAlignedBox();
}

template<class TRenderManager>
void DrawSkyDome<TRenderManager>::SetTexture(Hash texture)
{
	mTexture = mRenderManager.GetTextureManager().GetAsset(texture);
}

}

#endif // DRAWSKYDOME_H
