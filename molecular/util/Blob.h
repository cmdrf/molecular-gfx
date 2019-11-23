/*	Blob.h

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

#ifndef MOLECULAR_BLOB_H
#define MOLECULAR_BLOB_H

#include "NonCopyable.h"
#include <cstring>
#include <cstdint>

namespace molecular
{
namespace util
{

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

}
}

#endif // MOLECULAR_BLOB_H

