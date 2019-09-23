/*	FileTypeIdentification.h

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

#ifndef FILETYPEIDENTIFICATION_H
#define FILETYPEIDENTIFICATION_H

#include <cstddef>

namespace molecular
{

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

}

#endif // FILETYPEIDENTIFICATION_H
