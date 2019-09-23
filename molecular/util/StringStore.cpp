/*	StringStore.cpp
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "StringStore.h"
#include <stdexcept>

StringStore::StringStore(const void* fileData, size_t fileSize)
{
	LoadFromFile(fileData, fileSize);
}

void StringStore::LoadFromFile(const void* fileData, size_t fileSize)
{
	if(fileSize < sizeof(StringStoreFile))
		throw std::runtime_error("String map file truncated");
	const StringStoreFile* file = static_cast<const StringStoreFile*>(fileData);
	if(file->magic != StringStoreFile::kMagic)
		throw std::runtime_error("Unrecognized string map file");
	size_t headerAndEntriesSize = sizeof(StringStoreFile) + file->count * sizeof(StringStoreFile::Entry);
	size_t totalSize = headerAndEntriesSize + file->stringListSize;
	if(totalSize > fileSize)
		throw std::runtime_error("String map file truncated");

	mOffsets.clear();
	mOffsets.insert(file->entries, file->entries + file->count);
	const char* stringList = static_cast<const char*>(fileData) + headerAndEntriesSize;
	mStringList.assign(stringList, file->stringListSize);
}

const char* StringStore::GetString(uint32_t hash) const
{
	auto it = mOffsets.find(hash);
	if(it == mOffsets.end())
		throw std::runtime_error("Entry not found in StringStore");
	return &mStringList.at(it->second);
}

const char* StringStore::FindString(uint32_t hash) const noexcept
{
	auto it = mOffsets.find(hash);
	if(it == mOffsets.end())
		return nullptr;
	return &mStringList.at(it->second);
}
