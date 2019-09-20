/*	StreamStorage.h
	Copyright 2010-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef STREAMSTORAGE_H
#define STREAMSTORAGE_H

#include <stdlib.h> // size_t

/// Base class for data storage to be read from
/** Data storage can be be files, memory blocks and the like. */
class ReadStorage
{
public:
	virtual ~ReadStorage();
	virtual size_t Read(void* ptr, const size_t size) = 0;
	virtual bool EndOfData() = 0;
};

/// Base class for data storage to be written to
/** Data storage can be be files, memory blocks and the like. */
class WriteStorage
{
public:
	virtual ~WriteStorage() {}

	/// Write an array.
	virtual void Write(const void* ptr, const size_t size) = 0;
};

/// WriteStorage whose cursor can be set randomly
class RandomAccessWriteStorage : public WriteStorage
{
public:
	virtual size_t GetCursor() const = 0;
	virtual void SetCursor(size_t cursor) = 0;
};

#endif
