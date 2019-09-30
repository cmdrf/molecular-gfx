/*	StringStore.cpp

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
SOFTWARE.
*/

#include "StringStore.h"

#include <molecular/util/Hash.h>

#include <stdexcept>

namespace molecular
{

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

void StringStore::LoadFromText(const char* text, size_t size)
{
	const char* delimiters = ";\r\n";
	mStringList.reserve(size + 1);
	mStringList.assign(text, size);
	size_t currentBegin = 0;
	while(true)
	{
		size_t currentEnd = mStringList.find_first_of(delimiters, currentBegin);
		if(currentEnd == std::string::npos)
		{
			mStringList.push_back(0);
			Hash hash = HashUtils::MakeHash(mStringList.data() + currentBegin);
			mOffsets[hash] = currentBegin;
			break;
		}
		else
		{
			Hash hash = HashUtils::MakeHash(mStringList.data() + currentBegin, mStringList.data() + currentEnd);
			mOffsets[hash] = currentBegin;
			currentBegin = mStringList.find_first_not_of(delimiters, currentEnd);
			mStringList[currentEnd] = 0;
		}
		currentBegin = currentEnd + 1;
	}
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

}
