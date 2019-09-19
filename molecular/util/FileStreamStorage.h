/*	FileStreamStorage.h
	Copyright 2008-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef FILESTREAMSTORAGE_H
#define FILESTREAMSTORAGE_H

#include <cstdio>
#include <string>
#include <cstring>
#include <errno.h>
#include "StreamStorage.h"
#include <molecular/util/StringUtils.h>
#include "NonCopyable.h"
#include <stdexcept>
#include <assert.h>
#include <iostream>

/// A derived ReadStorage for reading regular files
class FileReadStorage : public ReadStorage
{
public:
	/// Deleted copy constructor
	FileReadStorage(const FileReadStorage& that) = delete;

	/// Move constructor
	FileReadStorage(FileReadStorage&& that) noexcept;

	/// Construct from file name
	explicit FileReadStorage(const char* filename);

	/// Construct from file name (std::string)
	explicit FileReadStorage(const std::string& filename);

	/// Destructor
	~FileReadStorage();

	/// Deleted copy assignment
	FileReadStorage& operator=(const FileReadStorage& that) = delete;

	/// Move assignment
	FileReadStorage& operator=(FileReadStorage&& that);

	inline size_t Read(void* ptr, const size_t size) override
	{
		assert(ptr);
		assert(mFile);
		return fread(ptr, 1, size, mFile);
	}

	inline void Skip(size_t size)
	{
		assert(mFile);
		fseek(mFile, long(size), SEEK_CUR);
	}

	bool EndOfData() override {assert(mFile); return feof(mFile) != 0;}

	size_t GetSize()
	{
		assert(mFile);
		long oldPos = ftell(mFile);
		fseek(mFile, 0, SEEK_END);
		long endPos = ftell(mFile);
		fseek(mFile, oldPos, SEEK_SET);
		return endPos;
	}

	size_t GetCursor() {return ftell(mFile);}

private:
	FILE* mFile = nullptr;
};
	
/// A derived WriteStorage for writing regular files
class FileWriteStorage : public RandomAccessWriteStorage, NonCopyable
{
public:
	explicit FileWriteStorage(const char* filename);
	explicit FileWriteStorage(const std::string& filename);
	~FileWriteStorage();

	inline void Write(const void* ptr, const size_t size) override
	{
		assert(mFile);
		assert(ptr);
		fwrite(ptr, 1, size, mFile);
	}

	size_t GetCursor() const override
	{
		assert(mFile);
		return ftell(mFile);
	}

	void SetCursor(size_t cursor) override
	{
		assert(mFile);
		if(fseek(mFile, static_cast<long>(cursor), SEEK_SET) != 0)
			throw std::runtime_error(std::string("fseek: ") + molecular::StringUtils::StrError(errno));
	}

private:
	FILE* mFile = nullptr;
};


#endif
