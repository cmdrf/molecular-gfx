/*	DdsFile.h

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

#ifndef MOLECULAR_DDSFILE_H
#define MOLECULAR_DDSFILE_H

#include "PixelFormat.h"
#include "NonCopyable.h"
#include "LittleEndianStream.h"
#include <stdexcept>
#include <vector>

namespace molecular
{

/// Four character code string literal
/** Little Endian. */
constexpr uint32_t operator"" _4CC(const char* str, size_t /*length*/)
{
//	static_assert(length == 4, "Only four characters allowed");
	return  uint32_t(str[0]) |
			(uint32_t(str[1]) << 8) |
			(uint32_t(str[2]) << 16) |
			(uint32_t(str[3]) << 24);
}

/// Interface to DirectDrawSurface files
/** @todo Linear non-compressed formats. */
class DdsFile : NonCopyable
{
public:
	/// Construct from memory block
	/** The memory must remain valid for calls to GetSingleImage. Useful for
		mmapped files. */
	DdsFile(const void* pointer, size_t size);

	~DdsFile();

	bool IsDataCompressed();
	PixelFormat GetFormat();

	/** e.g. 1 for regular images, 6 for cube maps. */
	unsigned int GetNumFaces() const {return mNumFaces;}

	/// Returns number of mipmap levels
	/** @return Number of mipmaps, or 1 for no mipmapping. */
	unsigned int GetNumMipmapLevels() const {return mHeader.mipMapCount;}

	/// Returns data pointer and sizes of a single image inside the file
	const void* GetSingleImage(unsigned int face, unsigned int mipmapLevel, unsigned int& outWidth, unsigned int& outHeight, size_t& outSize) const;

	/// Part of the file header for pixel format information
	struct PixelFormatHeader
	{
		/// Flags
		/** @see flags */
		enum
		{
			/// Texture contains alpha data
			/** aBitMask contains valid data. */
			kAlphaPixels = 0x1,

			/// Used in some older DDS files for alpha channel only uncompressed data .
			/** rgbBitCount contains the alpha channel bitcount; aBitMask contains valid data. */
			kAlpha = 0x2,

			/// Texture contains compressed RGB data.
			/** fourCC contains valid data. */
			kFourCc = 0x4,

			/// Texture contains uncompressed RGB data.
			/** rgbBitCount and the RGB masks (rBitMask, gBitMask, bBitMask) contain valid data. */
			kRgb = 0x40,
			kYuv = 0x200,
			kLuminance    = 0x20000,
			kAlphaPremult = 0b0000000000001000000000000000,
			kNormal       = 0b10000000000000000000000000000000

		};

		/// Values for fourCC
		/** @see fourCc */
		enum
		{
			kBc1Dxt1 = "DXT1"_4CC,
			kDxt2 = "DXT2"_4CC,
			kBc2Dxt3 = "DXT3"_4CC,
			kDxt4 = "DXT4"_4CC,
			kBc3Dxt5 = "DXT55"_4CC,
			kBc4U = "BC4U"_4CC, ///< Unsigned BC4 compressed
			kBc4S = "BC4S"_4CC, ///< Signed BC4 compressed
			kBc5U = "ATI2"_4CC, ///< Unsigned BC5 compressed
			kBc5S = "BC5S"_4CC, ///< Signed BC5 compressed
			kRgbg = "RGBG"_4CC,
			kGrgb = "GRBG"_4CC,
			kRgba16U = 36, // Or BGRA?
			kRgba16S = 110,
			kRFloat16 = 111,
			kRgFloat16 = 112,
			kRgbaFloat16 = 113,
			kRFloat32 = 114,
			kRgFloat32 = 115,
			kRgbaFloat32 = 116,
			kUyvy = "UYVY"_4CC,
			kYuy2 = "YUY2"_4CC,
			kCxvu8 = 117,
			kDxgi = "DX10"_4CC,

			/// Ericsson Texture Compression 1
			/** Unofficial. 64 bit blocks*/
			kEtc1 = "ETC1"_4CC,

			/// Ericsson Texture Compression 2
			/** Official? 64 bit blocks */
			kEtc2 = "ETC2"_4CC,

			/// Ericsson Texture Compression 2 with alpha channel
			/** Official? 128 bit blocks */
			kEtc2Alpha = "ETCA"_4CC
		};

		uint32_t size; ///< Size of the header in bytes
		uint32_t flags;
		uint32_t fourCc;
		uint32_t rgbBitCount;
		uint32_t rBitMask;
		uint32_t gBitMask;
		uint32_t bBitMask;
		uint32_t aBitMask;
	};
	static_assert(sizeof(PixelFormatHeader) == 32, "PixelFormatHeader has wrong size");

	/// DDS file header
	struct Header
	{
		/// flags
		enum
		{
			kCaps = 0x1,
			kHeight = 0x2,
			kWidth = 0x4,
			kPitch = 0x8,
			kPixelformat = 0x1000,
			kMipmapCount = 0x20000,
			kLinearSize = 0x80000,
			kDepth = 0x800000
		};

		/// caps
		enum
		{
			kComplex = 0x8,
			kMipmap = 0x400000,
			kTexture = 0x1000
		};

		enum
		{
			kCubemap = 0x200,
			kCubemapPositiveX = 0x400,
			kCubemapNegativeX = 0x800,
			kCubemapPositiveY = 0x1000,
			kCubemapNegativeY = 0x2000,
			kCubemapPositiveZ = 0x4000,
			kCubemapNegativeZ = 0x8000,
			kVolume = 0x200000,

			kCubemapAllFaces = kCubemapPositiveX | kCubemapNegativeX | kCubemapPositiveY | kCubemapNegativeY | kCubemapPositiveZ | kCubemapNegativeZ
		};

		uint32_t size; ///< Size of the header in bytes
		uint32_t flags;
		uint32_t height;
		uint32_t width;
		uint32_t pitchOrLinearSize;
		uint32_t depth;
		uint32_t mipMapCount;
		uint32_t reserved1[11];
		PixelFormatHeader pixelFormat;
		uint32_t caps;
		uint32_t caps2;
		uint32_t caps3;
		uint32_t caps4;
		uint32_t reserved2;
	};
	static_assert(sizeof(Header) == 124, "Header has wrong size");

private:
	/// Image data of one image inside the file
	struct MipmapLevel
	{
		MipmapLevel() : pointer(nullptr), size(0) {}
		uint8_t* pointer;
		size_t size;
		unsigned int width;
		unsigned int height;
	};

	size_t SingleImageSize(unsigned int width, unsigned int height);

	template<class Storage>
	void ReadHeader(Storage& storage);

	Header mHeader;
	bool mOwningBuffers;
	unsigned int mNumFaces = 1;

	std::vector<MipmapLevel> mFacesMipmapLevels;
};

template<class Storage>
void DdsFile::ReadHeader(Storage& storage)
{
	LittleEndianReadStream<Storage> stream(storage);

	uint32_t magic;
	stream.Read(magic);
	if(magic != 0x20534444)
		throw std::runtime_error("Data is no DDS file");

#if __LITTLE_ENDIAN__
	storage.Read(&mHeader, 124);
#else
	stream.Read(mHeader.size);
	stream.Read(mHeader.flags);
	stream.Read(mHeader.height);
	stream.Read(mHeader.width);
	stream.Read(mHeader.pitchOrLinearSize);
	stream.Read(mHeader.depth);
	stream.Read(mHeader.mipMapCount);
	for(int i = 0; i < 11; ++i)
		stream.Read(mHeader.reserved1[i]);
	stream.Read(mHeader.pixelFormat.size);
	stream.Read(mHeader.pixelFormat.flags);
	stream.Read(mHeader.pixelFormat.fourCc);
	stream.Read(mHeader.pixelFormat.rgbBitCount);
	stream.Read(mHeader.pixelFormat.rBitMask);
	stream.Read(mHeader.pixelFormat.gBitMask);
	stream.Read(mHeader.pixelFormat.bBitMask);
	stream.Read(mHeader.pixelFormat.aBitMask);
	stream.Read(mHeader.caps);
	stream.Read(mHeader.caps2);
	stream.Read(mHeader.caps3);
	stream.Read(mHeader.caps4);
	stream.Read(mHeader.reserved2);
#endif
	if(mHeader.size != 124)
		throw std::runtime_error("Invalid header data");

	if(!(mHeader.flags & (Header::kCaps | Header::kPixelformat | Header::kWidth | Header::kHeight)))
		throw std::runtime_error("Insufficient header data");

	if(!(mHeader.caps & Header::kTexture))
		throw std::runtime_error("No texture data present");

	mFacesMipmapLevels.resize(mNumFaces * mHeader.mipMapCount);
}

}

#endif // MOLECULAR_DDSFILE_H
