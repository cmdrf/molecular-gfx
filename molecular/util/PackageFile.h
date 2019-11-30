/*	PackageFile.h

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

#ifndef MOLECULAR_PACKAGEFILE_H
#define MOLECULAR_PACKAGEFILE_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <molecular/util/Hash.h>

namespace molecular
{
namespace util
{

/// PAK-like file structure
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

/// Indexes multiple package files
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

	/// Index contents of package file
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

	/// Add index entries
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
}

#endif // PACKAGEFILE_H
