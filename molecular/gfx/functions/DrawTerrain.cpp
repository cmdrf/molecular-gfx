/*	DrawTerrain.cpp
	Copyright 2014-2017 Fabian Herb
*/

#include "DrawTerrain.h"
#include <molecular/gfx/DefaultProgramData.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace Gfx
{

DrawTerrain::~DrawTerrain()
{
	for(int i = 0; i < kLodLevels; ++i)
		mRenderer.DestroyIndexBuffer(mPatchIndices[i]);
	mRenderer.DestroyVertexBuffer(mPatchVertices);
	mRenderer.DestroyTexture(mNormalMap);
	mRenderer.DestroyTexture(mHeightmap);
}

void DrawTerrain::Execute()
{
	if(mHeightmapWidth == 0 || mHeightmapHeight == 0)
		return;

	Binding<Uniform<Matrix4> > viewMatrix("viewMatrix"_H, this);
	Binding<Uniform<Matrix4> > modelMatrix("modelMatrix"_H, this);

	Binding<Attribute> patchVertex("terrainVertex"_H, this);
	Binding<Uniform<IntVector2> > start("terrainStart"_H, this);
	Binding<Uniform<Vector3> > scale("terrainScale"_H, this);
	Binding<Uniform<RenderCmdSink::Texture*> > heightmap("heightmapTexture"_H, this);
	Binding<Uniform<RenderCmdSink::Texture*> > normalMap("terrainNormalMap"_H, this);
	Binding<Uniform<unsigned int> > pickingColor("pickingColor"_H, this);

	*patchVertex = Attribute(mPatchVertices, 2, VertexAttributeInfo::kUInt16);
	**scale = Vector3(mXSize / mHeightmapWidth, mHeightScale, mYSize / mHeightmapHeight);
	**heightmap = mHeightmap;
	**normalMap = mNormalMap;
	**pickingColor = mPickingId;

	RenderCmdSink::Program* program = PrepareProgram();

	for(unsigned int y = 0; y <= mHeightmapHeight - kPatchSize; y += kPatchSize - 1)
	{
		for(unsigned int x = 0; x <= mHeightmapWidth - kPatchSize; x += kPatchSize - 1)
		{
			float xCenter = (x + kPatchSize/2) * (**scale)[0];
			float zCenter = (y + kPatchSize/2) * (**scale)[2];
			Vector3 patchCenter = (**viewMatrix * (**modelMatrix * Vector4(xCenter, 0, zCenter, 1))).Xyz();
			float patchViewerDistance = patchCenter.Length();
			int lodLevel = int(sqrt(patchViewerDistance * mLodFactor));
			if(lodLevel >= kLodLevels)
				lodLevel = kLodLevels - 1;
			else if(lodLevel < 0)
				lodLevel = 0;

			**start = IntVector2(x, y);
			start->Apply(program, "terrainStart"_H);
			mRenderer.Draw(mPatchIndices[lodLevel], mPatchInfos[lodLevel]);
		}
	}
}

util::AxisAlignedBox DrawTerrain::GetBounds() const
{
	return util::AxisAlignedBox(0, mMinHeight * mHeightScale, 0, mXSize, mMaxHeight * mHeightScale, mYSize);
}

void DrawTerrain::SetSize(float xSize, float ySize)
{
	mXSize = xSize;
	mYSize = ySize;
//	BoundsChanged();
}

void DrawTerrain::SetHeightmapData(unsigned int width, unsigned int height, const float* data)
{
	assert(mHeightmap);
	assert(mNormalMap);
	assert(data);

	mHeightmapWidth = width;
	mHeightmapHeight = height;

	mMinHeight = 100;
	mMaxHeight =-100;
	for(unsigned int i = 0; i < width * height; ++i)
	{
		if(data[i] > mMaxHeight)
			mMaxHeight = data[i];
		if(data[i] < mMinHeight)
			mMinHeight = data[i];
	}

	float sampleDistanceX = mXSize / mHeightmapWidth;
	float sampleDistanceY = mYSize / mHeightmapHeight;

	/* texture line byte count must be multiples of 4, so 3 bytes per pixel
		with an odd image width does not work: */
	const int components = 4;


	std::vector<uint8_t> normalMap(width * height * components);
	for(unsigned int y = 0; y < height; ++y)
	{
		for(unsigned int x = 0; x < width; ++x)
		{
			float center = data[y * width + x] * mHeightScale;
			float west = center;
			if(x > 0)
				west = data[y * width + (x-1)] * mHeightScale;
			float north = center;
			if(y > 0)
				north = data[(y-1) * width + x] * mHeightScale;
			float east = center;
			if(x < width-1)
				east = data[y * width + x +1] * mHeightScale;
			float south = center;
			if(y < height-1)
				south = data[(y+1) * width + x] * mHeightScale;

			Vector3 eastNormal(east - center, sampleDistanceX, 0);
			Vector3 westNormal(center - west, sampleDistanceX, 0);
			Vector3 northNormal(0, sampleDistanceY, center - north);
			Vector3 southNormal(0, sampleDistanceY, south - center);
			Vector3 normal = eastNormal.Normalized() + westNormal.Normalized() + northNormal.Normalized() + southNormal.Normalized();
			normal *= 0.25;
			normal *= Vector3(-1,1,-1); // ?
			uint8_t* pixel = &normalMap[(y*width + x) * components];
			pixel[0] = uint8_t(normal[0] * 127 + 128);
			pixel[1] = uint8_t(normal[1] * 127 + 128);
			pixel[2] = uint8_t(normal[2] * 127 + 128);
		}
	}
	// Extend size to multiples of 4:
	unsigned int evenWidth = width;
	while(evenWidth % 4)
		evenWidth++;
	unsigned int evenHeight = height;
	while(evenHeight % 4)
		evenHeight++;

	// Allocate larger images:
	mHeightmap->Store(evenWidth, evenHeight, nullptr, PF_L_FLOAT32);
	mNormalMap->Store(evenWidth, evenHeight, nullptr, PF_R8G8B8);

	// Store data as subimages:
	mHeightmap->Store(0, 0, width, height, data, PF_L_FLOAT32);
	mHeightmap->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kNearest);
	mHeightmap->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kNearest);

	mNormalMap->Store(0, 0, width, height, &normalMap[0], PF_R8G8B8A8);
	mNormalMap->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kNearest);
	mNormalMap->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kNearest);

//	BoundsChanged();
}

void DrawTerrain::SetTestData(unsigned int width, unsigned int height)
{
	std::vector<float> data(width * height);
	for(unsigned int y = 0; y < height; y++)
	{
		for(unsigned int x = 0; x < width; x++)
		{
			data[y * width + x] = sin(x * 0.1f) * sin(y * 0.1f);
		}
	}
	SetHeightmapData(width, height, &data[0]);
}

void DrawTerrain::BuildIndices(int lodLevel, std::vector<uint16_t>& indices, IndexBufferInfo& info)
{
	int skip = 1 << lodLevel;
	indices.clear();

	info.count = 0;
	info.buffer = lodLevel;
	info.mode = IndexBufferInfo::Mode::kTriangles;
	info.offset = 0;
	info.type = IndexBufferInfo::Type::kUInt16;

	for(uint16_t y = 0; y < kPatchSize-1; y += skip)
	{
		for(uint16_t x = 0; x < kPatchSize-1; x += skip)
		{
			indices.push_back(y * kPatchSize + x);
			indices.push_back((y+skip) * kPatchSize + x);
			indices.push_back(y * kPatchSize + (x+skip));
			indices.push_back(y * kPatchSize + (x+skip));
			indices.push_back((y+skip) * kPatchSize + x);
			indices.push_back((y+skip) * kPatchSize + (x+skip));

			info.count += 6;
		}
	}
}

}
}
