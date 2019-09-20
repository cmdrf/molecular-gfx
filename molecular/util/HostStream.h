/*	HostStream.h
	Copyright 2014 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef HOSTSTREAM_H
#define HOSTSTREAM_H

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
	
	void Write(const uint8_t value, const AttributeInfo* info = nullptr) override;
	void Write(const uint16_t value, const AttributeInfo* info = nullptr) override;
	void Write(const uint32_t value, const AttributeInfo* info = nullptr) override;
	void Write(const uint64_t value, const AttributeInfo* info = nullptr) override;
	void Write(const void* ptr, const size_t size, const AttributeInfo* info = nullptr) override;

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
	void Read(uint8_t&	value, const AttributeInfo* info = nullptr) override;
	void Read(uint16_t&	value, const AttributeInfo* info = nullptr) override;
	void Read(uint32_t&	value, const AttributeInfo* info = nullptr) override;
	void Read(uint64_t&	value, const AttributeInfo* info = nullptr) override;
	void Read(void*	ptr, const size_t size, const AttributeInfo* info = nullptr) override;

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
void HostWriteStream<Storage>::Write(const uint8_t value, const AttributeInfo* /*info*/)
{
	mStreamStorage.Write(&value, 1);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint16_t value, const AttributeInfo* /*info*/)
{
	mStreamStorage.Write(&value, 2);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint32_t value, const AttributeInfo* /*info*/)
{
	mStreamStorage.Write(&value, 4);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const uint64_t value, const AttributeInfo* /*info*/)
{
	mStreamStorage.Write(&value, 8);
}

template<class Storage>
void HostWriteStream<Storage>::Write(const void* ptr, const size_t size, const AttributeInfo* /*info*/)
{
	mStreamStorage.Write(ptr, size);
}

/*****************************************************************************/

template<class Storage>
void HostReadStream<Storage>::Read(uint8_t& value, const AttributeInfo* /*info*/)
{
	mStorage.Read(&value, 1);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint16_t& value, const AttributeInfo* /*info*/)
{
	mStorage.Read(&value, 2);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint32_t& value, const AttributeInfo* /*info*/)
{
	mStorage.Read(&value, 4);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint64_t& value, const AttributeInfo* /*info*/)
{
	mStorage.Read(&value, 8);
}

template<class Storage>
void HostReadStream<Storage>::Read(void* ptr, const size_t size, const AttributeInfo* /*info*/)
{
	mStorage.Read(ptr, size);
}

}
#endif
