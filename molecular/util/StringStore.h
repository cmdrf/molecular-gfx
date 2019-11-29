/*	StringStore.h

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

#ifndef MOLECULAR_STRINGSTORE_H
#define MOLECULAR_STRINGSTORE_H

#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <string>

namespace molecular
{
namespace util
{

/// Structure for files mapping hashes to strings
struct StringStoreFile
{
/*	struct Entry
	{
		uint32_t hash;
		uint32_t offset;
	};*/
	using Entry = std::pair<uint32_t, uint32_t>;
	static_assert(sizeof(Entry) == 8, "std::pair has additional elements");

	static const uint32_t kMagic = 0x807a0fc0;
	uint32_t magic;
	uint32_t count;
	uint32_t stringListSize;
	uint32_t reserved;
	Entry entries[0];
	// String list starts after entries
};

static_assert(sizeof(StringStoreFile) == 16, "Unexpected size of HashToStringMapFile");

/// Maps hashes to strings
class StringStore
{
public:
	/// Construct from file data
	/** @see StringStoreFile */
	StringStore(const void* fileData, size_t fileSize);

	/// Default constructor
	/** Use LoadFromFile() to load data! */
	StringStore() = default;

	/// Load from file data
	/** @see StringStoreFile */
	void LoadFromFile(const void* fileData, size_t fileSize);

	void LoadFromText(const char* text, size_t size);

	/// Get string corresponding to the given hash
	/** Throws if string was not found. */
	const char* GetString(uint32_t hash) const;

	const char* FindString(uint32_t hash) const noexcept;

	/// Get number of stored strings
	size_t GetStringCount() const {return mOffsets.size();}

	/// Begin iterator
	std::unordered_map<uint32_t, uint32_t>::const_iterator begin() const {return mOffsets.begin();}

	/// End iterator
	std::unordered_map<uint32_t, uint32_t>::const_iterator end() const {return mOffsets.end();}

private:
	std::unordered_map<uint32_t, uint32_t> mOffsets;
	std::string mStringList;
};

}
}

#endif
