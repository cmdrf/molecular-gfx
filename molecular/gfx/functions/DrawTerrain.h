/*	DrawTerrain.h
	Copyright 2012-2017 Fabian Herb
*/

#ifndef DRAWTERRAIN_H
#define DRAWTERRAIN_H

#include "DrawingFunction.h"

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
	AxisAlignedBox GetBounds() const override;

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

#endif // DRAWTERRAIN_H
