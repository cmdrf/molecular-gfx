/*	TetrahedronSpaceFile.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TETRAHEDRONSPACEFILE_H
#define TETRAHEDRONSPACEFILE_H

#include <cstdint>
#include <molecular/util/Matrix3.h>

namespace molecular
{

struct TetrahedronSpaceFile
{
	struct Tetrahedron
	{
		int16_t vertices[4];
		int16_t neighbours[4];
		Matrix<3, 4> matrix;
	};

	struct Vertex
	{
		Vector3 position;
		Matrix<3, 9> coefficients;
	};

	static const uint32_t kMagic = 0x801e14ae;
	static const uint32_t kVersion = 2;

	uint32_t magic;
	uint32_t version;
	uint32_t numPoints;
	uint32_t numTetrahedrons;
	uint32_t verticesOffset;
	uint32_t normalsOffset;
	Tetrahedron tetrahedra[0];

	const Vertex* GetVertices() const {return reinterpret_cast<const Vertex*>(reinterpret_cast<const uint8_t*>(this) + verticesOffset);}
	const Vector3* GetNormals() const {return reinterpret_cast<const Vector3*>(reinterpret_cast<const uint8_t*>(this) + normalsOffset);}
};

template<class TetrahedronIterator, class PositionIterator, class CoeffsIterator, class NormalsIterator, class Storage>
void WriteTetrahedronSpaceFile(
		TetrahedronIterator tetBegin, TetrahedronIterator tetEnd,
		PositionIterator posBegin, PositionIterator posEnd,
		CoeffsIterator coeffsBegin, CoeffsIterator coeffsEnd,
		NormalsIterator normalsBegin, NormalsIterator normalsEnd,
		Storage& outStorage)
{
	TetrahedronSpaceFile header;
	header.magic = TetrahedronSpaceFile::kMagic;
	header.version = TetrahedronSpaceFile::kVersion;
	header.numPoints = posEnd - posBegin;
	assert(header.numPoints == coeffsEnd - coeffsBegin);
	assert(header.numPoints == normalsEnd - normalsBegin);
	header.numTetrahedrons = tetEnd - tetBegin;
	header.verticesOffset = sizeof(TetrahedronSpaceFile) + header.numTetrahedrons * sizeof(TetrahedronSpaceFile::Tetrahedron);
	header.normalsOffset = header.verticesOffset + header.numPoints * sizeof(TetrahedronSpaceFile::Vertex);

	outStorage.Write(&header, sizeof(TetrahedronSpaceFile));
	for(auto it = tetBegin; it != tetEnd; ++it)
		outStorage.Write(&*it, sizeof(TetrahedronSpaceFile::Tetrahedron));
	auto pIt = posBegin;
	auto cIt = coeffsBegin;
	for(; pIt != posEnd && cIt != coeffsEnd; ++pIt, ++cIt)
	{
		TetrahedronSpaceFile::Vertex vertex{*pIt, *cIt};
		outStorage.Write(&vertex, sizeof(TetrahedronSpaceFile::Vertex));
	}

	for(auto it = normalsBegin; it != normalsEnd; ++it)
		outStorage.Write(&*it, sizeof(Vector3));
}

}

#endif // TETRAHEDRONSPACEFILE_H
