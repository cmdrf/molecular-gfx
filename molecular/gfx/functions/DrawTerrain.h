/*	DrawTerrain.h

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
SOFTWARE.
*/

#ifndef MOLECULAR_DRAWTERRAIN_H
#define MOLECULAR_DRAWTERRAIN_H

#include "DrawingFunction.h"

namespace molecular
{
namespace Gfx
{

/** @todo Frustum culling
	@todo Skirting
	@todo Mipmapping */
class DrawTerrain : public DrawingFunction
{
public:
	template<class TRenderManager>
	DrawTerrain(TRenderManager& manager);
	~DrawTerrain();

	void Execute() override;
	util::AxisAlignedBox GetBounds() const override;

	/** @todo Pass as parameter to SetHeightmapData(). */
	void SetHeightScale(float scale) {mHeightScale = scale;}
	void SetSize(float xSize, float ySize);
	void SetHeightmapData(unsigned int width, unsigned int height, const float* data);
	void SetTestData(unsigned int width, unsigned int height);
	void SetLodFactor(float factor) {mLodFactor = factor;}
	void SetPickingId(unsigned int id) {mPickingId = id;}

private:
	static void BuildIndices(int lodLevel, std::vector<uint16_t>& indices, IndexBufferInfo& info);

	RenderCmdSink::Texture* mHeightmap;
	RenderCmdSink::Texture* mNormalMap;
	static const int kLodLevels = 6;
	static const int kPatchSize = (1 << kLodLevels) + 1;
	RenderCmdSink::VertexBuffer* mPatchVertices;
	RenderCmdSink::IndexBuffer* mPatchIndices[kLodLevels];
	IndexBufferInfo mPatchInfos[kLodLevels];
	float mXSize, mYSize, mHeightScale;
	float mLodFactor;
	unsigned int mHeightmapWidth, mHeightmapHeight;
	float mMinHeight, mMaxHeight;
	unsigned int mPickingId;
};

template<class TRenderManager>
DrawTerrain::DrawTerrain(TRenderManager& manager) :
	DrawingFunction(manager),
	mHeightmap(nullptr),
	mNormalMap(nullptr),
	mPatchVertices(mRenderer.CreateVertexBuffer()),
	mXSize(10.0f),
	mYSize(10.0f),
	mHeightScale(1.0f),
	mLodFactor(0.05f),
	mHeightmapWidth(0),
	mHeightmapHeight(0),
	mMinHeight(-1.0f),
	mMaxHeight(1.0f),
	mPickingId(0)
{
	mHeightmap = mRenderer.CreateTexture();
	mNormalMap = mRenderer.CreateTexture();

	// Build vertex data:
	std::vector<uint16_t> patchVertexData(kPatchSize * kPatchSize * 2);
	for(uint16_t y = 0; y < kPatchSize; ++y)
	{
		for(uint16_t x = 0; x < kPatchSize; ++x)
		{
			patchVertexData[(y * kPatchSize + x) * 2] = x;
			patchVertexData[(y * kPatchSize + x) * 2 + 1] = y;
		}
	}
	mPatchVertices->Store(&patchVertexData.front(), patchVertexData.size() * sizeof(uint16_t));

	// Build index data:
	std::vector<uint16_t> indices;
	for(int i = 0; i < kLodLevels; ++i)
	{
		mPatchIndices[i] = mRenderer.CreateIndexBuffer();
		BuildIndices(i, indices, mPatchInfos[i]);
		mPatchIndices[i]->Store(&indices.front(), indices.size() * 2);
	}

/*	GLint num = 0;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &num);
	std::cerr << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: " << std::dec << num << std::endl;*/
}

}
}

#endif // DRAWTERRAIN_H
