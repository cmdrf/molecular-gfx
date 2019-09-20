/*	MemoryStreamStorage.cpp
	Copyright 2012-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "MemoryStreamStorage.h"

MemoryReadStorage::MemoryReadStorage(const void* pointer, size_t length) :
	mData(static_cast<const uint8_t*>(pointer)),
	mCursor(0),
	mLength(length)
{}

MemoryWriteStorage::MemoryWriteStorage(void* pointer, size_t length) :
	mData(static_cast<uint8_t*>(pointer)),
	mCursor(0),
	mLength(length)
{}

void MemoryWriteStorage::SetCursor(size_t cursor)
{
	mCursor = cursor;
}
