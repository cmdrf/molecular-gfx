/*	DrawTestSkeleton.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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
SOFTWARE.
*/

#ifndef MOLECULAR_DRAWTESTSKELETON_H
#define MOLECULAR_DRAWTESTSKELETON_H

#include "DrawingFunction.h"
#include <molecular/util/CharacterAnimation.h>

namespace molecular
{
namespace gfx
{

class DrawTestSkeleton : public DrawingFunction
{
public:
	template<class TRenderManager>
	DrawTestSkeleton(TRenderManager& manager);

	util::AxisAlignedBox GetBounds() const override {return util::AxisAlignedBox();}

	void SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount]);

protected:
	void HandleExecute(Scope& scope) override;

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

#endif // MOLECULAR_DRAWTESTSKELETON_H
