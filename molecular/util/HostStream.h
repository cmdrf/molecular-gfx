/*	HostStream.h

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

#ifndef MOLECULAR_HOSTSTREAM_H
#define MOLECULAR_HOSTSTREAM_H

#include "Stream.h"
#include "StreamStorage.h"

namespace molecular
{

/// Stream-interface to a WriteStorage that does no endian conversion.
template<class Storage>
class HostWriteStream : public WriteStream
{
public:
	HostWriteStream(Storage& streamStorage);
	
	void Write(const uint8_t value) override;
	void Write(const uint16_t value) override;
	void Write(const uint32_t value) override;
	void Write(const uint64_t value) override;
	void Write(const void* ptr, const size_t size) override;

	using WriteStream::Write;

private:
	Storage& mStreamStorage;
};

/// Stream-interface to a ReadStorage that does no endian conversion.
template<class Storage>
class HostReadStream : public ReadStream
{
public:
	HostReadStream(Storage& storage) : mStorage(storage){}
	void Read(uint8_t&	value) override;
	void Read(uint16_t&	value) override;
	void Read(uint32_t&	value) override;
	void Read(uint64_t&	value) override;
	void Read(void*	ptr, const size_t size) override;

	using ReadStream::Read;

private:
	Storage& mStorage;
};

/*****************************************************************************/

template<class Storage>
HostWriteStream<Storage>::HostWriteStream(Storage& streamStorage) :
	mStreamStorage(streamStorage)
{

}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint8_t value)
{
	mStreamStorage.Write(&value, 1);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint16_t value)
{
	mStreamStorage.Write(&value, 2);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint32_t value)
{
	mStreamStorage.Write(&value, 4);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint64_t value)
{
	mStreamStorage.Write(&value, 8);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const void* ptr, const size_t size)
{
	mStreamStorage.Write(ptr, size);
}

/*****************************************************************************/

template<class Storage>
void HostReadStream<Storage>::Read(uint8_t& value)
{
	mStorage.Read(&value, 1);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint16_t& value)
{
	mStorage.Read(&value, 2);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint32_t& value)
{
	mStorage.Read(&value, 4);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint64_t& value)
{
	mStorage.Read(&value, 8);
}

template<class Storage>
void HostReadStream<Storage>::Read(void* ptr, const size_t size)
{
	mStorage.Read(ptr, size);
}

}
#endif
