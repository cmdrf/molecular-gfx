/*	KtxFile.cpp

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

#include "KtxFile.h"
#include <stdexcept>
#include <cassert>
#include <algorithm>

namespace molecular
{
namespace util
{

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
	assert(mipmapLevel == 0 || static_cast<unsigned>(mipmapLevel) < GetNumberOfMipmapLevels());
	assert(face == 0 || static_cast<unsigned>(face) < numberOfFaces);
	assert(static_cast<unsigned>(arrayElement) < numberOfArrayElements);

	size_t singleImageSize = mMipLevels[mipmapLevel].imageSize;
	if(numberOfFaces != 6 || numberOfArrayElements != 0)
		singleImageSize /= (numberOfArrayElements * numberOfFaces);
	unsigned int subImage = arrayElement * numberOfFaces + face;
	const uint8_t* imageData = static_cast<const uint8_t*>(mFileData) + mMipLevels[mipmapLevel].offset + subImage * singleImageSize;
	return std::pair<const void*, std::size_t>(imageData, singleImageSize);
}

}
}
