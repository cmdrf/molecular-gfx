/*	DrawTestSkeleton.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DRAWTESTSKELETON_H
#define DRAWTESTSKELETON_H

#include "DrawingFunction.h"
#include <molecular/gfx/CharacterAnimation.h>

namespace molecular
{
namespace Gfx
{

class DrawTestSkeleton : public DrawingFunction
{
public:
	template<class TRenderManager>
	DrawTestSkeleton(TRenderManager& manager);

	void Execute() override;
	util::AxisAlignedBox GetBounds() const override {return util::AxisAlignedBox();}

	void SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount]);

private:
	RenderCmdSink::VertexBuffer* mVertexBuffer;
	VertexAttributeInfo mPositionInfo;
	RenderCmdSink::IndexBuffer* mIndexBuffer;
	IndexBufferInfo mIndicesInfo;
};


template<class TRenderManager>
DrawTestSkeleton::DrawTestSkeleton(TRenderManager& manager) :
	DrawingFunction(manager),
	mVertexBuffer(mRenderer.CreateVertexBuffer()),
	mPositionInfo(VertexAttributeInfo::kFloat, 3, 0, 0, 0),
	mIndexBuffer(mRenderer.CreateIndexBuffer())
{
	constexpr int lineCount = CharacterAnimation::kBoneCount;
	constexpr int indexCount = lineCount * 2;
	uint8_t indices[indexCount] = {0};
	for(int i = 2; i < lineCount; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = CharacterAnimation::kBoneParents[i];
	}
	mIndexBuffer->Store(indices, indexCount);
	mIndicesInfo.mode = IndexBufferInfo::Mode::kLines;
	mIndicesInfo.type = IndexBufferInfo::Type::kUInt8;
	mIndicesInfo.count = indexCount;
}

}
}

#endif // DRAWTESTSKELETON_H
