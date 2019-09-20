/*	KtxFile.h

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

#ifndef MOLECULAR_KTXFILE_H
#define MOLECULAR_KTXFILE_H

#include <cstdint>
#include <vector>

namespace molecular
{

/// Interface to Khronos KTX texture files.
/** @todo Test cube map and array textures. */
class KtxFile
{
public:
	struct Header {
		static const uint8_t kKtxIdentifier[12];

		uint8_t identifier[12];
		uint32_t endianness;
		uint32_t glType;
		uint32_t glTypeSize;
		uint32_t glFormat;
		uint32_t glInternalFormat;
		uint32_t glBaseInternalFormat;
		uint32_t pixelWidth;
		uint32_t pixelHeight;
		uint32_t pixelDepth;
		uint32_t numberOfArrayElements;
		uint32_t numberOfFaces;
		uint32_t numberOfMipmapLevels;
		uint32_t bytesOfKeyValueData;
	};

	/** @param fileData Pointer to file contents. Must be valid through the entire lifetime of this KtxFile. */
	KtxFile(const void* fileData, std::size_t fileSize);

	uint32_t GetGlType() const {return GetHeader()->glType;}
	uint32_t GetGlTypeSize() const {return GetHeader()->glTypeSize;}
	uint32_t GetGlFormat() const {return GetHeader()->glFormat;}
	uint32_t GetGlInternalFormat() const {return GetHeader()->glInternalFormat;}
	uint32_t GetGlBaseInternalFormat() const {return GetHeader()->glBaseInternalFormat;}
	uint32_t GetPixelWidth() const {return GetHeader()->pixelWidth;}
	uint32_t GetPixelHeight() const {return GetHeader()->pixelHeight;}
	uint32_t GetPixelDepth() const {return GetHeader()->pixelDepth;}
	uint32_t GetNumberOfArrayElements() const {return GetHeader()->numberOfArrayElements;}
	uint32_t GetNumberOfFaces() const {return GetHeader()->numberOfFaces;}
	uint32_t GetNumberOfMipmapLevels() const {return GetHeader()->numberOfMipmapLevels;}

	std::pair<const void*, std::size_t> GetImageData(int mipmapLevel, int face = 0, int arrayElement = 0);

private:
	const Header* GetHeader() const {return static_cast<const Header*>(mFileData);}

	const void* mFileData;

	struct MipLevel
	{
		uint32_t imageSize;
		unsigned int offset;
	};

	std::vector<MipLevel> mMipLevels;
};

}

#endif // MOLECULAR_KTXFILE_H
