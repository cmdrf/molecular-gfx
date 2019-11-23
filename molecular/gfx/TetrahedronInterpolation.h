/*	TetrahedronInterpolation.h

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

#ifndef MOLECULAR_GFX_TETRAHEDRONINTERPOLATION_H
#define MOLECULAR_GFX_TETRAHEDRONINTERPOLATION_H

#include <molecular/util/Blob.h>
#include <molecular/util/Vector4.h>
#include "TetrahedronSpaceFile.h"

namespace molecular
{
namespace gfx
{

using molecular::Vector4;

class TetrahedronInterpolation
{
public:
	void SetFileData(Blob&& fileData);

	util::Matrix<3, 9> GetShCoefficients(const Vector3& position, int& tetIndex) const;

	/** For debugging. */
	void GetTetrahedronCorners(int tetIndex, Vector3 outCorners[4], bool& outIsOuterCell) const;

	Vector4 GetCornerWeights(int tetIndex, const Vector3& position) const;

private:
	Vector4 GetLightProbeInterpolationWeights(const Vector3& position, int& tetIndex) const;
	Vector4 GetBarycentricCoordinates(const Vector3& p, const TetrahedronSpaceFile::Tetrahedron& tet) const;

	Blob mFileData;
	static const util::Matrix<3, 9> kZeros;
};

}
}

#endif // GFX_TETRAHEDRONINTERPOLATION_H
