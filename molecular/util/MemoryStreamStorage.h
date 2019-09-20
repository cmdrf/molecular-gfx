/*	MemoryStreamStorage.h

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

#ifndef MOLECULAR_MEMORYSTREAMSTORAGE_H
#define MOLECULAR_MEMORYSTREAMSTORAGE_H

#include "StreamStorage.h"
#include <stdexcept>
#include <cstring> // memcpy
#include <algorithm>

namespace molecular
{

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

}

#endif
