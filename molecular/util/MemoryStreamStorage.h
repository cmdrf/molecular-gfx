/*	MemoryStreamStorage.h
	Copyright 2012-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MEMORYSTREAMSTORAGE_H
#define MEMORYSTREAMSTORAGE_H

#include "StreamStorage.h"
#include <stdexcept>
#include <cstring> // memcpy
#include <algorithm>

/// ReadStorage interface to an existing block of memory
class MemoryReadStorage : public ReadStorage
{
public:
	MemoryReadStorage(const void* pointer, size_t length);

	inline size_t Read(void* ptr, const size_t size) override;

	inline bool EndOfData() override;

	inline void Skip(size_t size) {mCursor += size;}

	inline size_t GetCursor() const {return mCursor;}
	inline void Rewind() {mCursor = 0;}

	inline size_t GetSize() const {return mLength;}

private:
	const uint8_t* mData;
	size_t mCursor;
	size_t mLength;
};

/// WriteStorage interface to an existing block of memory
class MemoryWriteStorage : public RandomAccessWriteStorage
{
public:
	MemoryWriteStorage(void* pointer, size_t length);

	inline void Write(const void* ptr, const size_t size) override;

	inline size_t GetCursor() const override {return mCursor;}
	void SetCursor(size_t cursor) override;
	inline void Rewind() {mCursor = 0;}

	inline size_t GetSize() const {return mLength;}

private:
	uint8_t* mData;
	size_t mCursor;
	size_t mLength;
};

/*****************************************************************************/

inline size_t MemoryReadStorage::Read(void *ptr, const size_t size)
{
	if(mCursor >= mLength)
		throw std::range_error("MemoryReadStorage at end");

	size_t toRead = std::min(size, mLength - mCursor);

	memcpy(ptr, &mData[mCursor], toRead);
	mCursor += toRead;
	return toRead;
}

inline bool MemoryReadStorage::EndOfData()
{
	return (mCursor >= mLength);
}

/*****************************************************************************/

inline void MemoryWriteStorage::Write(const void *ptr, const size_t size)
{
	if(mCursor + size > mLength)
		throw std::range_error("MemoryWriteStorage at end");

	memcpy(&mData[mCursor], ptr, size);
	mCursor += size;
}

#endif
