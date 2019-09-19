/*	FileStreamStorage.cpp
	Copyright 2008-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "FileStreamStorage.h"

FileReadStorage::FileReadStorage(FileReadStorage&& that) noexcept :
	mFile(that.mFile)
{
	that.mFile = nullptr;
}

FileReadStorage::FileReadStorage(const char* filename)
{
#ifdef __STDC_LIB_EXT1__
	errno_t errno = fopen_s(&mFile, filename, "rb");
#else
	mFile = fopen(filename, "rb");
#endif
	if(!mFile)
		throw std::runtime_error(std::string(filename) + " could not be opened for reading: " + StringUtils::StrError(errno));
}

FileReadStorage::FileReadStorage(const std::string& filename) :
	FileReadStorage(filename.c_str())
{
}


FileReadStorage::~FileReadStorage()
{
	if(mFile)
		fclose(mFile);
}

FileReadStorage& FileReadStorage::operator=(FileReadStorage&& that)
{
	if(mFile)
		fclose(mFile);
	mFile = that.mFile;
	that.mFile = nullptr;
	return *this;
}

FileWriteStorage::FileWriteStorage(const char* filename)
{
#ifdef __STDC_LIB_EXT1__
	errno_t error = fopen_s(&mFile, filename, "wb");
#else
	mFile = fopen(filename, "wb");
#endif
	if(!mFile)
		throw std::runtime_error(std::string(filename) + " could not be opened for writing: " + StringUtils::StrError(errno));
}

FileWriteStorage::FileWriteStorage(const std::string& filename) :
	FileWriteStorage(filename.c_str())
{
}

FileWriteStorage::~FileWriteStorage()
{
	assert(mFile);
	fclose(mFile);
}
