/*	NmbMeshDataSource.h

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

#ifndef MOLECULAR_NMBMESHDATASOURCE_H
#define MOLECULAR_NMBMESHDATASOURCE_H

#include <molecular/gfx/MeshDataSource.h>
#include <molecular/util/NmbFile.h>

namespace molecular
{

class NmbMeshDataSource : public MeshDataSource
{
public:
	NmbMeshDataSource(NmbFile& file, const std::string &submesh = "");

	int PrepareVertexData(LayoutHint layout = kLayoutAny);

	int PrepareIndexData();

	unsigned int VertexBufferSize(int buffer);
	unsigned int IndexBufferSize(int buffer);
	const void* VertexBufferData(int buffer);
	const void* IndexBufferData(int buffer);
	util::AxisAlignedBox GetBounds() const;

private:
	NmbFile& mFile;
	std::string mSubmesh;
	std::vector<const NmbFile::VertexBuffer*> mVertexBuffers;
	std::vector<const NmbFile::IndexBuffer*> mIndexBuffers;
};

}

#endif // MOLECULAR_NMBMESHDATASOURCE_H
