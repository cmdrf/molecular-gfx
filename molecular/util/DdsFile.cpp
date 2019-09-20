/*	DdsFile.cpp

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
SOFTWARE.*/

#include "DdsFile.h"
#include "MemoryStreamStorage.h"
#include <cassert>

namespace molecular
{

DdsFile::DdsFile(const void *pointer, size_t size) : mOwningBuffers(false)
{
	MemoryReadStorage storage(pointer, size);
	ReadHeader(storage);

	uint8_t* currentPointer = reinterpret_cast<uint8_t*>(const_cast<void*>(pointer)) + 128;

	for(unsigned int face = 0; face < mNumFaces; face++)
	{
		unsigned int width = mHeader.width;
		unsigned int height = mHeader.height;
		for(unsigned int lvl = 0; lvl < mHeader.mipMapCount; ++lvl)
		{
			size_t imageSize = SingleImageSize(width, height);
			MipmapLevel& ml = mFacesMipmapLevels[face * mNumFaces + lvl];

			ml.width = width;
			ml.height = height;
			ml.size = imageSize;
			ml.pointer = currentPointer;

			currentPointer += imageSize;
			if(width > 1)
				width /= 2;
			if(height > 1)
				height /= 2;
		}
	}
}

DdsFile::~DdsFile()
{
	if(mOwningBuffers)
	{
		std::vector<MipmapLevel>::iterator it;
		for(it = mFacesMipmapLevels.begin(); it != mFacesMipmapLevels.end(); ++it)
			delete [] it->pointer;
	}
}

bool DdsFile::IsDataCompressed()
{
	return (mHeader.pixelFormat.flags & PixelFormatHeader::kFourCc)
			&& !(mHeader.pixelFormat.flags & (PixelFormatHeader::kRgb | PixelFormatHeader::kAlphaPixels));
}

PixelFormat DdsFile::GetFormat()
{
	if(mHeader.pixelFormat.flags & PixelFormatHeader::kFourCc)
	{
		switch(mHeader.pixelFormat.fourCc)
		{
		case PixelFormatHeader::kRgbaFloat16: return PF_RGBA_FLOAT16;
		case PixelFormatHeader::kRgbaFloat32: return PF_RGBA_FLOAT32;
		}
		if(mHeader.pixelFormat.flags & PixelFormatHeader::kNormal)
		{
			switch(mHeader.pixelFormat.fourCc)
			{
			case PixelFormatHeader::kBc1Dxt1: return PF_RGBA_DXT1_LINEAR;
			case PixelFormatHeader::kBc2Dxt3: return PF_RGBA_DXT3_LINEAR;
			case PixelFormatHeader::kBc3Dxt5: return PF_RGBA_DXT5_LINEAR;
			case PixelFormatHeader::kEtc1: return PF_RGB_ETC1_LINEAR;
			case PixelFormatHeader::kEtc2: return PF_RGB_ETC2_LINEAR;
			case PixelFormatHeader::kEtc2Alpha: return PF_RGBA_ETC2_LINEAR;
			}
		}
		else
		{
			switch(mHeader.pixelFormat.fourCc)
			{
			case PixelFormatHeader::kBc1Dxt1: return PF_RGBA_DXT1;
			case PixelFormatHeader::kBc2Dxt3: return PF_RGBA_DXT3;
			case PixelFormatHeader::kBc3Dxt5: return PF_RGBA_DXT5;
			case PixelFormatHeader::kEtc1: return PF_RGB_ETC1;
			case PixelFormatHeader::kEtc2: return PF_RGB_ETC2;
			case PixelFormatHeader::kEtc2Alpha: return PF_RGBA_ETC2;
			}
		}
	}
	else
	{
		PixelFormatHeader& pf = mHeader.pixelFormat;
		if(pf.rgbBitCount == 24)
		{
			if(pf.rBitMask == 0xff0000 && pf.gBitMask == 0xff00 && pf.bBitMask == 0xff)
				return PF_R8G8B8;
			else if(pf.rBitMask == 0xff && pf.gBitMask == 0xff00 && pf.bBitMask == 0xff0000)
				return PF_B8G8R8;
		}
		else if(pf.rgbBitCount == 32)
		{
			if(pf.rBitMask == 0xff000000 && pf.gBitMask == 0xff0000 && pf.bBitMask == 0xff00 && pf.aBitMask == 0xff)
				return PF_R8G8B8;
			else if(pf.bBitMask == 0xff000000 && pf.gBitMask == 0xff0000 && pf.rBitMask == 0xff00 && pf.aBitMask == 0xff)
				return PF_B8G8R8A8;
		}
	}

	return PF_UNSUPPORTED;
}

const void* DdsFile::GetSingleImage(unsigned int face, unsigned int mipmapLevel, unsigned int& outWidth, unsigned int& outHeight, size_t& outSize) const
{
	assert(face < mNumFaces);
	assert(mipmapLevel < GetNumMipmapLevels());
	const MipmapLevel& level = mFacesMipmapLevels[face * mNumFaces + mipmapLevel];
	outWidth = level.width;
	outHeight = level.height;
	outSize = level.size;
	return level.pointer;
}

size_t DdsFile::SingleImageSize(unsigned int width, unsigned int height)
{
	if(IsDataCompressed())
	{
		// All known texture compression algorithm store 4x4 pixels in either 64 or 128 bits
		unsigned int blockSize = 0; // Bytes per block
		switch(mHeader.pixelFormat.fourCc)
		{
		case PixelFormatHeader::kBc1Dxt1:
		case PixelFormatHeader::kBc4S:
		case PixelFormatHeader::kBc4U:
		case PixelFormatHeader::kEtc1:
		case PixelFormatHeader::kEtc2:
			blockSize = 8;
			break;
		default:
			blockSize = 16;
		}

		return ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
	}
	else
	{
		return width * height * mHeader.pixelFormat.rgbBitCount / 8;
	}
}

}
