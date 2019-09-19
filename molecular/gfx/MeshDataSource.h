/*	MeshDataSource.h
	Copyright 2012-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MESHDATASOURCE_H
#define MESHDATASOURCE_H

#include <vector>
#include <molecular/util/BufferInfo.h>
#include <molecular/util/AxisAlignedBox.h>

/// Interface to a source of mesh data
/** @deprecated Better compile to MeshFile directly. */
class MeshDataSource
{
public:
	MeshDataSource() : mVertexDataSets(1) {}
	virtual ~MeshDataSource(){}

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

	virtual AxisAlignedBox GetBounds() const = 0;

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

#endif // MESHDATASOURCE_H
