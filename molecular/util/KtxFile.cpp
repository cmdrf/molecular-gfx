/*	KtxFile.cpp
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "KtxFile.h"
#include <stdexcept>
#include <cassert>
#include <algorithm>

const uint8_t KtxFile::Header::kKtxIdentifier[12] = {0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};


KtxFile::KtxFile(const void* fileData, std::size_t fileSize) :
	mFileData(fileData),
	mMipLevels(GetHeader()->numberOfMipmapLevels)
{
	assert(fileData);
	if(fileSize <= sizeof(Header))
		throw std::runtime_error("No data after KTX header");
	const Header* header = GetHeader();

	for(int i = 0; i < 12; ++i)
	{
		if(header->identifier[i] != Header::kKtxIdentifier[i])
			throw std::runtime_error("Invalid KTX identifier");
	}
	if(header->endianness != 0x04030201)
		throw std::runtime_error("Wrong endianess in KTX file");

	if(header->numberOfFaces != 1 && header->numberOfFaces != 6)
		throw std::runtime_error("Invalid number of faces in KTX file");

	const uint8_t* bytes = static_cast<const uint8_t*>(fileData);
	unsigned int offset = sizeof(Header) + header->bytesOfKeyValueData;
	for(unsigned int i = 0; i < header->numberOfMipmapLevels; ++i)
	{
		if(offset + 4 > fileSize)
			throw std::runtime_error("KTX file too small");
		uint32_t imageSize = *reinterpret_cast<const uint32_t*>(bytes + offset);
		offset += 4;
		mMipLevels[i].offset = offset;
		mMipLevels[i].imageSize = imageSize;
		imageSize += 3 - (imageSize + 3) % 4; // Padding
		if(header->numberOfArrayElements < 2)
			offset += imageSize * header->numberOfFaces;
		else
			offset += imageSize;
	}
}

std::pair<const void*, std::size_t> KtxFile::GetImageData(int mipmapLevel, int face, int arrayElement)
{
	const unsigned int numberOfArrayElements = std::max(1u, GetNumberOfArrayElements());
	const unsigned int numberOfFaces = GetNumberOfFaces();
	assert(mipmapLevel == 0 || mipmapLevel < GetNumberOfMipmapLevels());
	assert(face == 0 || face < numberOfFaces);
	assert(arrayElement < numberOfArrayElements);

	size_t singleImageSize = mMipLevels[mipmapLevel].imageSize;
	if(numberOfFaces != 6 || numberOfArrayElements != 0)
		singleImageSize /= (numberOfArrayElements * numberOfFaces);
	unsigned int subImage = arrayElement * numberOfFaces + face;
	const uint8_t* imageData = static_cast<const uint8_t*>(mFileData) + mMipLevels[mipmapLevel].offset + subImage * singleImageSize;
	return std::pair<const void*, std::size_t>(imageData, singleImageSize);
}
