/*	DrawTerrain.cpp

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

#include "DrawTerrain.h"
#include <molecular/gfx/DefaultProgramData.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace gfx
{

DrawTerrain::~DrawTerrain()
{
	for(int i = 0; i < kLodLevels; ++i)
		mRenderer.DestroyIndexBuffer(mPatchIndices[i]);
	mRenderer.DestroyVertexBuffer(mPatchVertices);
	mRenderer.DestroyTexture(mNormalMap);
	mRenderer.DestroyTexture(mHeightmap);
}

void DrawTerrain::HandleExecute(Scope& scope)
{
	if(mHeightmapWidth == 0 || mHeightmapHeight == 0)
		return;

	const Matrix4 viewMatrix = *scope.Get<Uniform<Matrix4>>("cameraViewMatrix"_H);
	const Matrix4 modelMatrix = *scope.Get<Uniform<Matrix4>>("modelMatrix"_H);

	scope.Set("terrainVertex"_H, Attribute(mPatchVertices, 2, VertexAttributeInfo::kUInt16));
	scope.Set("heightmapTexture"_H, Uniform<RenderCmdSink::Texture*>(mHeightmap));
	scope.Set("terrainNormalMap"_H, Uniform<RenderCmdSink::Texture*>(mNormalMap));
	scope.Set("pickingColor"_H, Uniform<unsigned int>(mPickingId));

	Uniform<IntVector2>& start = scope.Bind<Uniform<IntVector2>>("terrainStart"_H);
	Uniform<Vector3>& scale = scope.Bind<Uniform<Vector3>>("terrainScale"_H);
	//dbgPatchCounter for debug purposes to draw patch borders
	Uniform<int>& dbgPatchCounter = scope.Bind<Uniform<int>>("dbgPatchCounter"_H);

	*scale = Vector3(mXSize / mHeightmapWidth, mHeightScale, mYSize / mHeightmapHeight);

	RenderCmdSink::Program* program = PrepareProgram(scope);

	int patch = 0;
	for(unsigned int y = 0; y <= mHeightmapHeight - kPatchSize; y += kPatchSize - 1)
	{
		for(unsigned int x = 0; x <= mHeightmapWidth - kPatchSize; x += kPatchSize - 1)
		{
			float xCenter = (x + kPatchSize/2) * (*scale)[0];
			float zCenter = (y + kPatchSize/2) * (*scale)[2];
			Vector3 patchCenter = (viewMatrix * (modelMatrix * Vector4(xCenter, 0, zCenter, 1))).Xyz();
			float patchViewerDistance = patchCenter.Length();
			int lodLevel = int(sqrt(patchViewerDistance * mLodFactor));
			if(lodLevel >= kLodLevels)
				lodLevel = kLodLevels - 1;
			else if(lodLevel < 0)
				lodLevel = 0;

			*start = IntVector2(x, y);
			*dbgPatchCounter = (patch % 10);
			patch++;
			start.Apply(program, "terrainStart"_H);
			dbgPatchCounter.Apply(program, "dbgPatchCounter"_H);
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
			data[y * width + x] = sin(x * 0.1f) * sin(y * 0.1f) * 20;
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
			indices.push_back(y * kPatchSize + (x+skip));
			indices.push_back((y+skip) * kPatchSize + x);
			indices.push_back((y+skip) * kPatchSize + x);
			indices.push_back(y * kPatchSize + (x+skip));
			indices.push_back((y+skip) * kPatchSize + (x+skip));

			info.count += 6;
		}
	}
}

}
}
