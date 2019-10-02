/*	FileTypeIdentification.cpp

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

#include "FileTypeIdentification.h"
#include <cstdint>
#include <cstring>

namespace molecular
{
namespace FileTypeIdentification
{

bool HasMagicNumber(const void* data, size_t size, uint32_t magic)
{
	if(size < 4)
		return false;
	const uint32_t* dwords = static_cast<const uint32_t*>(data);
	return (dwords[0] == magic);
}

bool IsKtx(const void* data, size_t size)
{
	if(size < 12)
		return false;
	static const uint8_t ktxIdentifier[12] = {0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};
	const uint8_t* bytes = static_cast<const uint8_t*>(data);
	for(int i = 0; i < 12; ++i)
	{
		if(bytes[i] != ktxIdentifier[i])
			return false;
	}
	return true;
}

bool IsDds(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 0x20534444);
}

bool IsTga(const void* data, size_t size)
{
	const char footer[] = "TRUEVISION-XFILE.";
	if(size < sizeof(footer))
		return false; // TGA file too short
	const char* bytes = static_cast<const char*>(data) + size - sizeof(footer);
	return (strncmp(bytes, footer, sizeof(footer)) == 0);
}

bool IsCompiledMesh(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 0x8e8e54f1);
}

bool IsNmb(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 1);
}

bool IsWav(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 0x46464952); // "RIFF"
}

bool IsOpus(const void* data, size_t size)
{
	if(size < 36)
		return false;
	const char* f = static_cast<const char*>(data);
	if(std::memcmp(f, "OggS", 4) != 0)
		return false;
	if(std::memcmp(f + 28, "OpusHead", 8) != 0)
		return false;
	return true;
}

bool IsHashSet(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 0x088a5ef1);
}

bool IsStringStore(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 0x807a0fc0);
}

bool IsPackage(const void *data, size_t size)
{
	return HasMagicNumber(data, size, 0x506ac4a6);
}

bool IsMeshBoundsCollection(const void* data, size_t size)
{
	return HasMagicNumber(data, size, 0x505eb0f1);
}

FileType Identify(const void* data, size_t size)
{
	if(IsDds(data, size))
		return kDds;
	if(IsKtx(data, size))
		return kKtx;
	if(IsTga(data, size))
		return kTga;

	return kUnknown;
}

}
}
