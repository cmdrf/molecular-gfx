/*	TgaFile.h

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

#ifndef MOLECULAR_TGAFILE_H
#define MOLECULAR_TGAFILE_H

#include <stdexcept>
#include <molecular/util/PixelFormat.h>
#include <sstream>

namespace molecular
{

/// Interface to TGA files
/** Indexed and/or RLE images not supported. Does not copy any data. */
class TgaFile2
{
public:
	/** All uint8_t because padding cannot be disabled with alignas. */
	struct Header
	{
		uint8_t idLength = 0, colorMapType = 0, imageType = kNoData;
		uint8_t paletteBegin0, paletteBegin1;
		uint8_t paletteLength0, paletteLength1 = 0;
		uint8_t paletteEntrySize = 0; ///< Size of palette that follows the header
		uint8_t xOrigin0; ///< X origin least significant byte
		uint8_t xOrigin1; ///< X origin most significant byte
		uint8_t yOrigin0; ///< Y origin least significant byte
		uint8_t yOrigin1; ///< Y origin most significant byte
		uint8_t width0; ///< Width least significant byte
		uint8_t width1; ///< Width most significant byte
		uint8_t height0;
		uint8_t height1;
		uint8_t bitsPerPixel = 0, imageDescriptor = 0;
	};
	static_assert(sizeof(Header) == 18, "Unwanted padding in TgaFile2");

	/** @param data File contents. Must be valid while this object is in use. */
	TgaFile2(const void* data, size_t size) :
		mData(data)
	{
		if(size < sizeof(Header))
			throw std::runtime_error("TGA file too short");

		const Header* header = static_cast<const Header*>(mData);
		switch(header->imageType)
		{
		case kRgb:
		case kMono:
			break;
		case kIndexed: throw std::runtime_error("Unsupported TGA format: Indexed");
		case kIndexedRle: throw std::runtime_error("Unsupported TGA format: Indexed RLE");
		case kRgbRle: throw std::runtime_error("Unsupported TGA format: RGB RLE");
		case kMonoRle: throw std::runtime_error("Unsupported TGA format: Mono RLE");
		default:
			{
				std::ostringstream oss;
				oss << int(header->imageType);
				throw std::runtime_error("Unsupported TGA format " + oss.str());
			}
		}

		if(header->idLength + header->paletteEntrySize + GetImageSize() > size)
			throw std::runtime_error("TGA file too short");
	}

	uint16_t GetWidth() const
	{
		const Header* header = static_cast<const Header*>(mData);
		return uint16_t(header->width1) << 8 | header->width0;
	}

	uint16_t GetHeight() const
	{
		const Header* header = static_cast<const Header*>(mData);
		return uint16_t(header->height1) << 8 | header->height0;
	}

	PixelFormat GetFormat() const
	{
		const Header* header = static_cast<const Header*>(mData);
		if(header->imageType == kRgb && header->bitsPerPixel == 24)
			return PF_B8G8R8;
		else if(header->imageType == kRgb && header->bitsPerPixel == 32)
			return PF_B8G8R8A8;
		else if(header->imageType == kMono && header->bitsPerPixel == 8)
			return PF_L8;
		else if(header->imageType == kMono && header->bitsPerPixel == 16)
			return PF_L8A8; // Oder andersrum?
		return PF_NONE;
	}

	const void* GetImageData() const
	{
		const Header* header = static_cast<const Header*>(mData);
		return static_cast<const uint8_t*>(mData) + header->idLength + header->paletteEntrySize;
	}

	size_t GetImageSize() const
	{
		const Header* header = static_cast<const Header*>(mData);
		return GetWidth() * GetHeight() * (header->bitsPerPixel / 8);
	}

	bool IsUpsideDown() const
	{
		const Header* header = static_cast<const Header*>(mData);
		return !(header->imageDescriptor & 0x20);
	}

	unsigned int GetBytesPerPixel() const
	{
		return static_cast<const Header*>(mData)->bitsPerPixel / 8;
	}

private:
	enum
	{
		kNoData = 0,
		kIndexed = 1,
		kRgb = 2,
		kMono = 3,
		kIndexedRle = 9,
		kRgbRle = 10,
		kMonoRle = 11
	};

	const void* mData;
};

}

#endif // MOLECULAR_TGAFILE_H
