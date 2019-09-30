/*	FileServer.h

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

#ifndef MOLECULAR_FILESERVER_H
#define MOLECULAR_FILESERVER_H

#include <molecular/Config.h>
#include <molecular/util/Blob.h>
#include <molecular/util/FileStreamStorage.h>
#include <molecular/util/SyncFileLoad.h>
#include <molecular/util/StringStore.h>
#include <molecular/util/Hash.h>
#include <molecular/util/PackageFile.h>
#include <molecular/util/Logging.h>
#include <molecular/util/StringUtils.h>

#include <limits>
#include <functional>
#include <cassert>
#include <string>

namespace molecular
{
/** Supports PAK-like archive files and plain files. */
template<class TFileLoader>
class FileServer
{
public:
	static const size_t kMaxPathLength = 256;

	using FileLoader = TFileLoader;

	template<class TQueue>
	FileServer(FileLoader& fileLoader, const std::string& root, TQueue& backgroundQueue) :
		mFileLoader(fileLoader),
		mRoot(root)
	{
		try
		{
			Blob contentsFile = SyncFileLoad((mRoot + "contents.mss").c_str(), fileLoader, backgroundQueue);
			mDirectoryContents.LoadFromFile(contentsFile.GetData(), contentsFile.GetSize());
		}
		catch(std::exception&)
		{
			try
			{
				FileReadStorage storage(mRoot + "contents.txt");
				auto fileContents = StringUtils::FromStorage(storage);
				mDirectoryContents.LoadFromText(fileContents.data(), fileContents.size());
			}
			catch (std::exception& e)
			{
				LOG(WARNING) << "Loading directory contents file failed: " << e.what();
			}
		}
	}

	template<class TQueue>
	void AddPackageFile(const std::string& path, TQueue& backgroundQueue)
	{
		try
		{
			const size_t headerSize = sizeof(PackageFile);
			Blob headerData = SyncFileLoad(path.c_str(), mFileLoader, backgroundQueue, 0, headerSize);
			const PackageFile* header = static_cast<const PackageFile*>(headerData.GetData());
			if(header->magic != PackageFile::kMagic)
				throw std::runtime_error("File is not a package file.");
			const size_t entriesSize = header->count * sizeof(PackageFile::Entry);
			Blob entryData = SyncFileLoad(path.c_str(), mFileLoader, backgroundQueue, headerSize, entriesSize);
			const PackageFile::Entry* entriesBegin = static_cast<const PackageFile::Entry*>(entryData.GetData());
			mPackageFileDirectory.Populate(path, entriesBegin, entriesBegin + header->count);
			LOG(INFO) << "Package file " + path + " loaded. " << header->count << " files.";
		}
		catch(std::exception& e)
		{
			throw std::runtime_error("Loading of package file " + path + " failed: " + e.what());
		}
	}

	/// Read entire file and pass contents to handler
	/** Without FinishFlag.
		@param file Path to file
		@param handler Function to call when file is loaded.
		@param handlerQueue Queue to run handler in. */
	template<class TQueue>
	void ReadFile(
			Hash file,
			std::function<void (Blob&)> handler,
			TQueue& handlerQueue) const
	{
		char path[kMaxPathLength];
		size_t offset = 0, size = 0;
		GetFileLocation(file, path, offset, size);
		mFileLoader.ReadFile(path, handler, handlerQueue, offset, size);
	}

	/// Read entire file and pass contents to handler
	/** With FinishFlag
		@param path Path to file
		@param handler Function to call when file is loaded.
		@param handlerQueue Queue to run handler in. */
	template<class TQueue>
	void ReadFile(
			Hash file,
			std::function<void (Blob&)> handler,
			TQueue& handlerQueue,
			typename TQueue::FinishFlag& finishFlag) const
	{
		char path[kMaxPathLength];
		size_t offset = 0, size = 0;
		GetFileLocation(file, path, offset, size);
		mFileLoader.ReadFile(path, handler, handlerQueue, finishFlag, offset, size);
	}

	template<class TQueue>
	Blob ReadFileSync(Hash file, TQueue& backgroundQueue) const
	{
		char path[kMaxPathLength];
		size_t offset = 0, size = 0;
		GetFileLocation(file, path, offset, size);
		return SyncFileLoad(path, mFileLoader, backgroundQueue, offset, size);
	}


private:
	void GetFileLocation(Hash file, char outPath[kMaxPathLength], size_t& outOffset, size_t& outSize) const
	{
		if(const char* path = mDirectoryContents.FindString(file))
		{
			strncpy(outPath, mRoot.c_str(), kMaxPathLength);
			strncpy(outPath + mRoot.size(), path, kMaxPathLength - mRoot.size());
			outOffset = 0;
			outSize = 0;
		}
		else if(auto entry = mPackageFileDirectory.FindEntry(file))
		{
			strncpy(outPath, entry->file.c_str(), kMaxPathLength);
			outOffset = entry->offset;
			outSize = entry->size;
		}
		else
			throw std::runtime_error("File not found");
	}

	FileLoader& mFileLoader;
	std::string mRoot;
	StringStore mDirectoryContents;
	PackageFileDirectory mPackageFileDirectory;
};
}

#endif // MOLECULAR_FILESERVER_H
