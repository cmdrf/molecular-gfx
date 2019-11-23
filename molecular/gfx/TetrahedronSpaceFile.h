/*	TetrahedronSpaceFile.h

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

#ifndef MOLECULAR_TETRAHEDRONSPACEFILE_H
#define MOLECULAR_TETRAHEDRONSPACEFILE_H

#include <cstdint>
#include <molecular/util/Matrix3.h>

namespace molecular
{
namespace gfx
{
struct TetrahedronSpaceFile
{
	struct Tetrahedron
	{
		int16_t vertices[4];
		int16_t neighbours[4];
		util::Matrix<3, 4> matrix;
	};

	struct Vertex
	{
		Vector3 position;
		util::Matrix<3, 9> coefficients;
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
}

#endif // TETRAHEDRONSPACEFILE_H
