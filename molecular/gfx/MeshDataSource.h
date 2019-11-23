/*	MeshDataSource.h

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

#ifndef MOLECULAR_MESHDATASOURCE_H
#define MOLECULAR_MESHDATASOURCE_H

#include <vector>
#include <molecular/util/BufferInfo.h>
#include <molecular/util/AxisAlignedBox.h>

namespace molecular
{
namespace gfx
{
using namespace util;

/// Interface to a source of mesh data
/** @deprecated Better compile to MeshFile directly. */
class MeshDataSource
{
public:
	MeshDataSource() : mVertexDataSets(1) {}
	virtual ~MeshDataSource() = default;

	enum LayoutHint
	{
		kLayoutSeperate,
		kLayoutInterleavedSeperatePositions,
		kLayoutConcatenated,
		kLayoutInterleaved,
		kLayoutAny
	};

	/** @returns Number of vertex buffers. */
	virtual int PrepareVertexData(LayoutHint layout = kLayoutAny) = 0;

	/** @returns Number of index buffers. */
	virtual int PrepareIndexData() = 0;

	/// Returns vertex buffer size in bytes
	/** @param buffer Index of the vertex buffer to query
		@returns Size of the given vertex buffer in bytes. */
	virtual unsigned int VertexBufferSize(int buffer) = 0;

	/// Returns index buffer size in bytes
	/** @param buffer Index of the index buffer to query
		@returns Size of the given index buffer in bytes. */
	virtual unsigned int IndexBufferSize(int buffer) = 0;

	/** @param buffer Index of the vertex buffer to query
		@returns Pointer to the vertex buffer data. */
	virtual const void* VertexBufferData(int buffer) = 0;

	/** @param buffer Index of the index buffer to query
		@returns Pointer to the index buffer data. */
	virtual const void* IndexBufferData(int buffer) = 0;

	virtual util::AxisAlignedBox GetBounds() const = 0;

	unsigned int GetNumVertexDataSets() const {return unsigned(mVertexDataSets.size());}
	const std::vector<VertexAttributeInfo>& GetVertexBufferInfos(unsigned int vertexDataSet = 0) const {return mVertexDataSets.at(vertexDataSet).vertexBufferInfos;}
	const std::vector<IndexBufferInfo>& GetIndexBufferInfos() const {return mIndexBufferInfos;}
	unsigned int GetNumVertices(unsigned int vertexDataSet = 0) const {return mVertexDataSets.at(vertexDataSet).numVertices;}

protected:
	void SetNumVertexDataSets(unsigned int count) {mVertexDataSets.resize(count);}
	void SetNumVertices(unsigned int numVertices, unsigned int vertexDataSet = 0) {mVertexDataSets.at(vertexDataSet).numVertices = numVertices;}
	void AddVertexAttributeInfo(const VertexAttributeInfo& info, unsigned int vertexDataSet = 0) {mVertexDataSets.at(vertexDataSet).vertexBufferInfos.push_back(info);}
	void AddIndexBufferInfo(const IndexBufferInfo& info) {mIndexBufferInfos.push_back(info);}
	VertexAttributeInfo& GetVertexAttributeInfo(unsigned int index, unsigned int vertexDataSet = 0) {return mVertexDataSets.at(vertexDataSet).vertexBufferInfos.at(index);}

private:
	struct VertexDataSet
	{
		VertexDataSet() : numVertices(0) {}

		std::vector<VertexAttributeInfo> vertexBufferInfos;
		unsigned int numVertices;
	};

	std::vector<VertexDataSet> mVertexDataSets;

	std::vector<IndexBufferInfo> mIndexBufferInfos;
};

}
}

#endif // MOLECULAR_MESHDATASOURCE_H
