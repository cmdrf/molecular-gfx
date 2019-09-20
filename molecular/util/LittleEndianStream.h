/*	LittleEndianStream.h

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

#ifndef MOLECULAR_LITTLEENDIANSTREAM_H
#define MOLECULAR_LITTLEENDIANSTREAM_H

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#include "HostStream.h"

// clang identifies as GCC 4.2
#if !defined(__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define LittleEndianWriteStream HostWriteStream
#define LittleEndianReadStream HostReadStream
#else

namespace molecular
{
// template typedefs only available in C++11
template<class Storage>
using LittleEndianReadStream = HostReadStream<Storage>;

template<class Storage>
using LittleEndianWriteStream = HostWriteStream<Storage>;
}
#endif
#else

#include "Stream.h"
#include "StreamStorage.h"

/// Wrapper to a StreamStorage for data in little endian format
template<class Storage = ReadStorage>
class LittleEndianReadStream : public ReadStream
{
public:
	LittleEndianReadStream(Storage& storage) : mStorage(storage) {}
	
	void Read(uint8_t&	value, const AttributeInfo* info = NULL);
	void Read(uint16_t&	value, const AttributeInfo* info = NULL);
	void Read(uint32_t&	value, const AttributeInfo* info = NULL);
	void Read(uint64_t&	value, const AttributeInfo* info = NULL);
	void Read(void* ptr, const size_t size, const AttributeInfo* info = NULL);

	using ReadStream::Read;

private:
	Storage& mStorage;
};

template<class Storage = WriteStorage>
class LittleEndianWriteStream : public WriteStream
{
public:
	LittleEndianWriteStream(Storage& storage) : mStorage(storage) {}

	void Write(const uint8_t value, const AttributeInfo* info = NULL);
	void Write(const uint16_t value, const AttributeInfo* info = NULL);
	void Write(const uint32_t value, const AttributeInfo* info = NULL);
	void Write(const uint64_t value, const AttributeInfo* info = NULL);
	void Write(const void* ptr, const size_t size, const AttributeInfo* info = NULL);

	using WriteStream::Write;

private:
	Storage& mStorage;
};

/*****************************************************************************/

template<class Storage>
void LittleEndianReadStream<Storage>::Read(uint8_t& value, const AttributeInfo* /*info*/)
{
	mStorage.Read(&value, 1);
}

template<class Storage>
void LittleEndianReadStream<Storage>::Read(uint16_t& value, const AttributeInfo* /*info*/)
{
	uint8_t tmp[2];
	mStorage.Read(tmp, 2);
	value = (uint16_t)tmp[0] | ((uint16_t)tmp[1] << 8);
}

template<class Storage>
void LittleEndianReadStream<Storage>::Read(uint32_t& value, const AttributeInfo* /*info*/)
{
	uint8_t tmp[4];
	mStorage.Read(tmp, 4);
	value = tmp[0] | ((uint32_t)tmp[1] << 8) | ((uint32_t)tmp[2] << 16) | ((uint32_t)tmp[3] << 24);
}

template<class Storage>
void LittleEndianReadStream<Storage>::Read(uint64_t& value, const AttributeInfo* /*info*/)
{
	uint8_t tmp[8];
	mStorage.Read(tmp, 8);
	value = tmp[0];
	value |= (uint64_t)tmp[1] << 8;
	value |= (uint64_t)tmp[2] << 16;
	value |= (uint64_t)tmp[3] << 24;
	value |= (uint64_t)tmp[4] << 32;
	value |= (uint64_t)tmp[5] << 40;
	value |= (uint64_t)tmp[6] << 48;
	value |= (uint64_t)tmp[7] << 56;
}

template<class Storage>
void LittleEndianReadStream<Storage>::Read(void* ptr, const size_t size, const AttributeInfo* /*info*/)
{
	mStorage.Read(ptr, size);
}

/*****************************************************************************/

template<class Storage>
void LittleEndianWriteStream<Storage>::Write(const uint8_t value, const AttributeInfo* /*info*/)
{
	mStorage.Write(value);
}

template<class Storage>
void LittleEndianWriteStream<Storage>::Write(const uint16_t value, const AttributeInfo* /*info*/)
{
	mStorage.Write((uint8_t)(value & 0xFF));
	mStorage.Write((uint8_t)((value >> 8) & 0xFF));
}

template<class Storage>
void LittleEndianWriteStream<Storage>::Write(const uint32_t value, const AttributeInfo* /*info*/)
{
	uint8_t tmp[4];
	tmp[3] = ((value >> 24) & 0xFF);
	tmp[2] = ((value >> 16) & 0xFF);
	tmp[1] = ((value >> 8) & 0xFF);
	tmp[0] = (value & 0xFF);
	mStorage.Write(tmp, 4);
}

template<class Storage>
void LittleEndianWriteStream<Storage>::Write(const uint64_t value, const AttributeInfo* /*info*/)
{
	uint8_t tmp[8];
	tmp[0] = value & 0xFF;
	tmp[1] = (value >> 8) & 0xFF;
	tmp[2] = (value >> 16) & 0xFF;
	tmp[3] = (value >> 24) & 0xFF;
	tmp[4] = (value >> 32) & 0xFF;
	tmp[5] = (value >> 40) & 0xFF;
	tmp[6] = (value >> 48) & 0xFF;
	tmp[7] = (value >> 56) & 0xFF;
	mStorage.Write(tmp, 8);
}

template<class Storage>
void LittleEndianWriteStream<Storage>::Write(const void* ptr, const size_t size, const AttributeInfo* /*info*/)
{
	mStorage.Write(ptr, size);
}

#endif

#endif
