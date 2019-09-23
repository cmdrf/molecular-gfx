/*	StringStore.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef HASHTOSTRINGMAP_H
#define HASHTOSTRINGMAP_H

#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <string>

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

#endif // HASHTOSTRINGMAP_H
