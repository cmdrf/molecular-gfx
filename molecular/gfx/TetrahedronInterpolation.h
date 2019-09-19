/*	TetrahedronInterpolation.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFX_TETRAHEDRONINTERPOLATION_H
#define GFX_TETRAHEDRONINTERPOLATION_H

#include <molecular/util/Blob.h>
#include <molecular/util/Vector4.h>
#include "TetrahedronSpaceFile.h"

namespace molecular
{
namespace Gfx
{

using molecular::Vector4;

class TetrahedronInterpolation
{
public:
	void SetFileData(Blob&& fileData);

	Matrix<3, 9> GetShCoefficients(const Vector3& position, int& tetIndex) const;

	/** For debugging. */
	void GetTetrahedronCorners(int tetIndex, Vector3 outCorners[4], bool& outIsOuterCell) const;

	Vector4 GetCornerWeights(int tetIndex, const Vector3& position) const;

private:
	Vector4 GetLightProbeInterpolationWeights(const Vector3& position, int& tetIndex) const;
	Vector4 GetBarycentricCoordinates(const Vector3& p, const TetrahedronSpaceFile::Tetrahedron& tet) const;

	Blob mFileData;
	static const Matrix<3, 9> kZeros;
};

}
}

#endif // GFX_TETRAHEDRONINTERPOLATION_H
