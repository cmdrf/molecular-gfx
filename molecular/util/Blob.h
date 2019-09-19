/*	Blob.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef BLOB_H
#define BLOB_H

#include "NonCopyable.h"
#include <cstring>
#include <cstdint>

/// Holds binary data
/** Contents are not initialized. Movable, non-copyable. */
class Blob : NonCopyable
{
public:
	Blob() : mData(nullptr), mSize(0) {}

	/// Construct from existing data
	Blob(const void* data, size_t size) :
		mData(new uint8_t[size]),
		mSize(size)
	{
		memcpy(mData, data, size);
	}

	/// Construct from size
	/** Contents are not initialized. */
	explicit Blob(size_t size) :
		mData(new uint8_t[size]),
		mSize(size)
	{
	}

	Blob(Blob&& that) noexcept :
		mData(that.mData),
		mSize(that.mSize)
	{
		that.mData = nullptr;
		that.mSize = 0;
	}

	~Blob() {delete [] mData;}

	Blob& operator=(Blob&& that) noexcept
	{
		delete [] mData;
		mData = that.mData;
		mSize = that.mSize;
		that.mData = nullptr;
		that.mSize = 0;
		return *this;
	}

	const uint8_t* GetBytes() const { return mData;}
	uint8_t* GetBytes() { return mData;}
	const void* GetData() const {return mData;}
	void* GetData() {return mData;}
	size_t GetSize() const {return mSize;}

private:
	uint8_t* mData;
	size_t mSize;
};

#endif // BLOB_H

