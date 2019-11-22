/*	NmbFile.h

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

#ifndef MOLECULAR_UTIL_NMBFILE_H
#define MOLECULAR_UTIL_NMBFILE_H

#include <molecular/util/LittleEndianStream.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <list>

namespace molecular
{
namespace util
{

/// NVidia geometry file
/** This is used in many NVidia demos. Specification is not publicly available,
	this has been reverse engineered. */
class NmbFile
{
public:
	template<class Storage>
	NmbFile(Storage& storage);

	struct IndexBuffer
	{
		enum Operation
		{
			kUnknown = 0,
			kTriangles = 3,
			kTriangleStrip = 5
		};

		Operation operation;
		std::vector<uint16_t> indices;
	};

	struct VertexBuffer
	{
		std::string name;
		int elementCount;
		int elementSize;
		std::vector<float> values;
	};

	struct Mesh
	{
		std::string name;
		std::vector<VertexBuffer> vertexBuffers;
		std::vector<IndexBuffer> indexBuffers;
	};

	struct FixedUniform
	{
		std::string name;
		float values[3];
	};

	struct Program
	{
		std::string name;
		std::string source;
		std::string entryFunction;
		std::vector<FixedUniform> mUniforms;
	};

	const std::list<Mesh>& GetMeshes() const {return mMeshes;}

private:

	/// Chunk IDs
	enum
	{
		kTexture = 0x20000100,
		kRenderTexture = 0x20000101,
		kCubemap = 0x20000301,

		kLight = 0x30000002,
		kProgram = 0x40000000,

		kUniformValue = 0x41000010,
		kUniformMatrixRef = 0x41000031,
		kUniformRef = 0x41000032,
		kUniformWorldEyePos = 0x41000033,

		kClamp = 0x50000019,

		kPolySurfaceShape = 0x70000031,
		kPolySurfaceShapeWithBones = 0x70000038,
		kPolyList = 0x70050002,

		kTransform = 0x80000001,

		kRenderPass = 0xc0000000

	};

	enum
	{
		kTriangles = 0x03,
		kTriangleStrips = 0x05
	};

	enum
	{
		kBeginMarker = 0xfffffffd,
		kEndMarker = 0xfffffffc
	};

	template<class Storage>
	void ReadPolySurfaceShape(Storage& storage, Mesh& mesh);

	template<class Storage>
	void ReadProgram(Storage& storage, Program& mesh);

	template<class Storage>
	void ReadString(Storage& storage, std::string& outStr);

	template<class Storage>
	void ReadMarker(Storage& storage, uint32_t marker = kEndMarker);

	std::list<Mesh> mMeshes;
};

template<class Storage>
NmbFile::NmbFile(Storage& storage)
{
	LittleEndianReadStream<Storage> stream(storage);
	uint32_t magic = 0, version1 = 0, version2 = 0;
	stream.Read(magic);

	if(magic != 1)
		throw std::runtime_error("Wrong magic number");

	stream.Read(version1);
	stream.Read(version2);

	while(!storage.EndOfData())
	{
		uint32_t marker = 0;
		stream.Read(marker);
		if(marker == kEndMarker)
			break;

		if((marker & 0xff000000) == 0)
			continue;

		ReadMarker(storage, kBeginMarker);

		std::string name;
		ReadString(storage, name);
//		std::cerr << "name: " << name << std::endl;

		switch(marker)
		{
		case kPolySurfaceShape:
			mMeshes.push_back(Mesh());
			mMeshes.back().name = name;
			ReadPolySurfaceShape(storage, mMeshes.back());
			break;

		case kProgram:
			{
				Program program;
				ReadProgram(storage, program);
			}
			break;

		default:
			throw std::runtime_error("Unknown chunk \"" + name + "\"");
		}
	}
}

template<class Storage>
void NmbFile::ReadPolySurfaceShape(Storage& storage, Mesh &mesh)
{
	LittleEndianReadStream<Storage> stream(storage);
	const int elementSizes[8] = {1, 2, 3, 4, 2, 3, 0, 0};

	uint8_t null = 0;
	stream.Read(null);
	uint32_t unknown1 = 0, unknown2 = 0;
	uint32_t  a2vCount = 0;
	stream.Read(unknown1);
	stream.Read(unknown2);
	stream.Read(a2vCount);

	mesh.vertexBuffers.resize(a2vCount);
	for(unsigned int i = 0; i < a2vCount; ++i)
	{
		VertexBuffer& vertBuffer = mesh.vertexBuffers[i];

		uint16_t a2vType = 0, unknown3a = 0;
		uint32_t unknown3b = 0;
		stream.Read(a2vType);
		stream.Read(unknown3a);
		stream.Read(unknown3b);

		std::string name;
		ReadString(storage, name);

		uint32_t unknown4 = 0, unknown5 = 0, unknown6 = 0;
		uint32_t elementCount = 0;
		stream.Read(unknown4);
		stream.Read(unknown5);
		stream.Read(unknown6);
		stream.Read(elementCount);

		unsigned int elementSize = elementSizes[a2vType & 0x7];
		if(elementSize == 0)
			throw std::runtime_error("Unknown element size");

		vertBuffer.name = name;
		vertBuffer.elementCount = elementCount;
		vertBuffer.elementSize = elementSize;
		vertBuffer.values.resize(elementCount * elementSize);

		for(size_t i = 0; i < elementCount * elementSize; ++i)
		{
			stream.Read(vertBuffer.values[i]);
		}

		ReadMarker(storage, kEndMarker);
	}

	uint32_t unknown7 = 0;
	uint32_t indexBufferCount = 0;
	stream.Read(unknown7);
	stream.Read(indexBufferCount);

	mesh.indexBuffers.resize(indexBufferCount);
	for(unsigned int i = 0; i < indexBufferCount; ++i)
	{
		IndexBuffer& indexBuffer = mesh.indexBuffers[i];

		uint32_t unknown8, unknown9, operation, unknown11, indexCount;
		stream.Read(unknown8);
		stream.Read(unknown9);
		stream.Read(operation);
		stream.Read(unknown11);
		stream.Read(indexCount);

		switch(operation)
		{
		case kTriangles:
			indexBuffer.operation = IndexBuffer::kTriangles;
			break;

		case kTriangleStrips:
			indexBuffer.operation = IndexBuffer::kTriangleStrip;
			break;

		default:
			indexBuffer.operation = IndexBuffer::kUnknown;
		}

		indexBuffer.indices.resize(indexCount);

		for(size_t i = 0; i < indexCount; ++i)
			stream.Read(indexBuffer.indices[i]);

		ReadMarker(storage, kEndMarker);
	}
	ReadMarker(storage, kEndMarker);
}

template<class Storage>
void NmbFile::ReadProgram(Storage& storage, Program& program)
{
	LittleEndianReadStream<Storage> stream(storage);

	uint32_t unknown1 = 0; // always 0
	stream.Read(unknown1);
	ReadString(storage, program.source);

	uint32_t unknown2 = 0, unknown3 = 0, unknown4 = 0;
	stream.Read(unknown2); // always 1
	stream.Read(unknown3); // fp30: 0x11; vp30: 5
	stream.Read(unknown4); // always 2
	ReadString(storage, program.entryFunction);

	uint32_t marker = 0;
	stream.Read(marker);
	if(marker != kEndMarker)
	{
		uint32_t unknown5 = 0; // param set count? always 1
		uint32_t unknown6 = 0; // 0x42000000
		uint32_t unknown7 = 0; // 0
		stream.Read(unknown5);
		stream.Read(unknown6);
		stream.Read(unknown7);
		std::string paramSetName;
		ReadString(storage, paramSetName);
		uint32_t unknown8 = 0; // 1
		uint32_t uniformCount = 0;
		stream.Read(unknown8);
		stream.Read(uniformCount);
		for(unsigned int i = 0; i < uniformCount; ++i)
		{
			uint32_t type = 0;
			uint32_t unknown9 = 0; // 0
			std::string uniformName;

			stream.Read(type);
			stream.Read(unknown9);
			ReadString(storage, uniformName);

			if(type == kUniformMatrixRef)
			{
				uint32_t unknown10 = 0; // 1
				uint32_t unknown11 = 0; // 0
				uint32_t unknown12 = 0; // 2
				std::string refName;
				uint32_t unknown13 = 0; // 3
				uint32_t unknown14 = 0; // 5

				stream.Read(unknown10);
				stream.Read(unknown11);
				stream.Read(unknown12);
				ReadString(storage, refName);
				stream.Read(unknown13);
				stream.Read(unknown14);
			}
			else if(type == kUniformRef)
			{
				uint32_t unknown10 = 0; // 1
				std::string refName;
				uint32_t unknown11 = 0; // 2
				uint32_t unknown12 = 0; // 0
				uint32_t unknown13 = 0; // 3
				uint32_t unknown14 = 0; // 1
				uint32_t unknown15 = 0; // 4
				uint32_t unknown16 = 0; // 0

				stream.Read(unknown10);
				ReadString(storage, refName);
				stream.Read(unknown11);
				stream.Read(unknown12);
				stream.Read(unknown13);
				stream.Read(unknown14);
				stream.Read(unknown15);
				stream.Read(unknown16);
			}
			else if(type == kUniformWorldEyePos)
			{
				uint32_t unknown10 = 0; // 1
				uint32_t unknown11 = 0; // 4
				uint32_t unknown12 = 0; // 2
				uint32_t unknown13 = 0; // 0

				stream.Read(unknown10);
				stream.Read(unknown11);
				stream.Read(unknown12);
				stream.Read(unknown13);
			}
			else if(type == kUniformValue)
			{
				uint32_t unknown10 = 0; // 1
				uint32_t unknown11 = 0; // 0
				uint32_t unknown12 = 0; // 0
				uint32_t unknown13 = 0; // 0
				uint32_t unknown14 = 0; // 0
				uint32_t unknown15 = 0; // 2
				uint32_t unknown16 = 0; // 0
				uint32_t unknown17 = 0; // 0
				uint32_t unknown18 = 0; // 0
				uint32_t unknown19 = 0; // 0
				uint32_t unknown20 = 0; // 3
				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32_t unknown21 = 0; // 0, w?

				stream.Read(unknown10);
				stream.Read(unknown11);
				stream.Read(unknown12);
				stream.Read(unknown13);
				stream.Read(unknown14);
				stream.Read(unknown15);
				stream.Read(unknown16);
				stream.Read(unknown17);
				stream.Read(unknown18);
				stream.Read(unknown19);
				stream.Read(unknown20);
				stream.Read(x);
				stream.Read(y);
				stream.Read(z);
				stream.Read(unknown21);

//				std::cerr << uniformName << ": (" << x << ", " << y << ", " << z << ")" << std::endl;
			}

			ReadMarker(storage, kEndMarker);
		}
		ReadMarker(storage, kEndMarker);
		ReadMarker(storage, kEndMarker);
	}
}

template<class Storage>
void NmbFile::ReadString(Storage& storage, std::string& outStr)
{
	LittleEndianReadStream<Storage> stream(storage);

	uint32_t strLength = 0;
	stream.Read(strLength);

	outStr.resize(strLength);
	for(unsigned int i = 0; i < strLength; ++i)
	{
		uint8_t character = 0;
		stream.Read(character);
		outStr[i] = character;
	}
}

template<class Storage>
void NmbFile::ReadMarker(Storage& storage, uint32_t marker)
{
	LittleEndianReadStream<Storage> stream(storage);

	uint32_t beginMarker = 0;
	stream.Read(beginMarker);
	if(beginMarker != marker)
		throw std::runtime_error("Marker missed");
}

}
}

#endif // NMBFILE_H
