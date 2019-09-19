/*	NmbMeshDataSource.cpp
	Copyright 2013-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "NmbMeshDataSource.h"
#include "Logging.h"

NmbMeshDataSource::NmbMeshDataSource(NmbFile& file, const std::string& submesh) :
	mFile(file),
	mSubmesh(submesh)
{
	const std::list<NmbFile::Mesh>& meshes = mFile.GetMeshes();

	if(submesh.empty())
		SetNumVertexDataSets(meshes.size());

	int mesh = 0;
	for(auto& it: meshes)
	{
		if(!mSubmesh.empty() && mSubmesh != it.name)
			continue;
		SetNumVertices(it.vertexBuffers.at(0).values.size(), mesh);

		for(auto& vertexBuffer: it.vertexBuffers)
		{
			VertexAttributeInfo info;
			info.type = VertexAttributeInfo::kFloat;
			info.components = vertexBuffer.elementSize;
			LOG(DEBUG) << vertexBuffer.name;
			if(vertexBuffer.name == "a2v.objCoord" || vertexBuffer.name == "a2v.worldCoord")
				info.semantic = VertexAttributeInfo::kPosition;
			else if(vertexBuffer.name == "a2v.objNormal" || vertexBuffer.name == "a2v.worldNormal")
				info.semantic = VertexAttributeInfo::kNormal;
			else if(vertexBuffer.name == "a2v.tex" || vertexBuffer.name == "a2v.diffuse" || vertexBuffer.name == "a2v.c_texCoord")
				info.semantic = VertexAttributeInfo::kTextureCoords;
			else
			{
				LOG(ERROR) << "Unbekannt: " << vertexBuffer.name;
				info.semantic = VertexAttributeInfo::kUnknown;
//				assert(false);
			}

			info.buffer = mVertexBuffers.size();

			AddVertexAttributeInfo(info, mesh);
			mVertexBuffers.push_back(&vertexBuffer);
		}

		for(auto& indexBuffer: it.indexBuffers)
		{
			IndexBufferInfo info;
			info.type = IndexBufferInfo::Type::kUInt16;
			switch(indexBuffer.operation)
			{
			case NmbFile::IndexBuffer::kTriangles:
				info.mode = IndexBufferInfo::Mode::kTriangles;
				break;

			case NmbFile::IndexBuffer::kTriangleStrip:
				info.mode = IndexBufferInfo::Mode::kTriangleStrip;
				break;

			case NmbFile::IndexBuffer::kUnknown:
				info.mode = IndexBufferInfo::Mode::kPoints;
			}

			info.buffer = mIndexBuffers.size();
			info.count = indexBuffer.indices.size();
			info.vertexDataSet = mesh;

			AddIndexBufferInfo(info);
			mIndexBuffers.push_back(&indexBuffer);
		}

		++mesh;
	}
}

int NmbMeshDataSource::PrepareVertexData(LayoutHint layout)
{
	return mVertexBuffers.size();
}

int NmbMeshDataSource::PrepareIndexData()
{
	return mIndexBuffers.size();
}

unsigned int NmbMeshDataSource::VertexBufferSize(int buffer)
{
	return mVertexBuffers.at(buffer)->values.size() * sizeof(float);
}

unsigned int NmbMeshDataSource::IndexBufferSize(int buffer)
{
	return mIndexBuffers.at(buffer)->indices.size() * 2;
}

const void* NmbMeshDataSource::VertexBufferData(int buffer)
{
	return &mVertexBuffers.at(buffer)->values.front();
}

const void* NmbMeshDataSource::IndexBufferData(int buffer)
{
	return &mIndexBuffers.at(buffer)->indices.front();
}

AxisAlignedBox NmbMeshDataSource::GetBounds() const
{
	return AxisAlignedBox(); // TODO
}
