/*	NmbMeshDataSource.cpp

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

#include "NmbMeshDataSource.h"
#include "Logging.h"

namespace molecular
{

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

util::AxisAlignedBox NmbMeshDataSource::GetBounds() const
{
	return AxisAlignedBox(); // TODO
}

}
