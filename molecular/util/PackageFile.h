/*	PackageFile.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PACKAGEFILE_H
#define PACKAGEFILE_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <molecular/util/Hash.h>

namespace molecular
{

struct PackageFile
{
	struct Entry
	{
		uint32_t name;
		uint32_t size;
		uint64_t offset;
	};

	const static uint32_t kMagic = 0x506ac4a6;
	uint32_t magic;
	uint32_t count;
	Entry entries[0];
};
static_assert(sizeof(PackageFile) == 8, "Unexpected size of PackageFile");
static_assert(sizeof(PackageFile::Entry) == 16, "Unexpected size of PackageFile::Entry");

class PackageFileDirectory
{
public:
	struct Entry
	{
		Entry(const std::string& file, size_t offset, size_t size) : file(file), offset(offset), size(size) {}
		Entry() = default;
		std::string file;
		size_t offset;
		size_t size;
	};

	template<class TStorage>
	void ReadFromFile(TStorage& storage, const std::string& filename)
	{
		PackageFile header;
		storage.Read(&header, sizeof(PackageFile));
		if(header.magic != PackageFile::kMagic)
			throw std::runtime_error("File is not a package file");
		std::vector<PackageFile::Entry> entries(header.count);
		storage.Read(entries.data(), sizeof(PackageFile::Entry) * header.count);
		for(auto& entry: entries)
		{
			mEntries[entry.name] = Entry(filename, entry.offset, entry.size);
		}
	}

	/** @param begin Iterator to PackageFile::Entry */
	template<class TIterator>
	void Populate(const std::string& filename, TIterator begin, TIterator end)
	{
		for(TIterator it = begin; it != end; ++it)
			mEntries[it->name] = Entry(filename, it->offset, it->size);
	}

	Entry GetEntry(Hash name) const {return mEntries.at(name);}
	Entry GetEntry(const char* name) const {return mEntries.at(HashUtils::MakeHash(name));}
	const Entry* FindEntry(Hash name) const noexcept
	{
		auto it = mEntries.find(name);
		if(it == mEntries.end())
			return nullptr;
		else
			return &(it->second);
	}

private:
	std::unordered_map<Hash, Entry> mEntries;
};

}

#endif // PACKAGEFILE_H
