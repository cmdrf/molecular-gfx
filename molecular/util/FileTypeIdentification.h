/*	FileTypeIdentification.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef FILETYPEIDENTIFICATION_H
#define FILETYPEIDENTIFICATION_H

#include <cstddef>

/// Functions for identifying file types from files contents
namespace FileTypeIdentification
{

enum FileType
{
	kUnknown = 0,
	kTga,
	kKtx,
	kDds
};

bool IsTga(const void* data, size_t size); ///< Checks if given file is a Targa image file
bool IsDds(const void* data, size_t size); ///< Checks if given file is a DirectDraw Surface texture file
bool IsKtx(const void* data, size_t size); ///< Checks if given file is a Khronos texture file
bool IsCompiledMesh(const void* data, size_t size);
bool IsWav(const void* data, size_t size);
bool IsOpus(const void* data, size_t size);
bool IsHashSet(const void* data, size_t size);
bool IsStringStore(const void* data, size_t size);
bool IsPackage(const void* data, size_t size);
bool IsMeshBoundsCollection(const void* data, size_t size);

FileType Identify(const void* data, std::size_t size);

}

#endif // FILETYPEIDENTIFICATION_H
