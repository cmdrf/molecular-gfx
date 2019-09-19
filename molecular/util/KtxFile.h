/*	KtxFile.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef KTXFILE_H
#define KTXFILE_H

#include <cstdint>
#include <vector>

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

#endif // KTXFILE_H
