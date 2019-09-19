/*	SetShCoefficients.cpp
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "SetShCoefficients.h"
#include <molecular/util/SphericalHarmonics.h>
#include <molecular/gfx/TetrahedronInterpolation.h>

using namespace molecular;

namespace molecular
{
namespace Gfx
{

void SetShCoefficients::Execute()
{
	Binding<Uniform<Matrix4>> modelMatrix("modelMatrix"_H, this);
	Vector3 position = (**modelMatrix).GetTranslation();
	if(!position.IsAlmostZero()) // Hack to exclude static geometry
	{
		Binding<Uniform<Vector3, 9>> lightingShCoeffs("lightingShCoeffs"_H, this);
		Matrix3 modelRotationInv = (**modelMatrix).GetUpperLeft3x3().GetRotation().Transposed();
		Matrix<3, 9> coeffs = mInterpolation.GetShCoefficients(position, mTetrahedronIndex);

		float factor = 0.5f / kPi_f;// std::sqrt(kPi_f);
		coeffs *= factor;
		float redTransformedCoeffs[9] = {0};
		float greenTransformedCoeffs[9] = {0};
		float blueTransformedCoeffs[9] = {0};

		SphericalHarmonics::RotateOrder3(redTransformedCoeffs, coeffs[0], modelRotationInv);
		SphericalHarmonics::RotateOrder3(greenTransformedCoeffs, coeffs[1], modelRotationInv);
		SphericalHarmonics::RotateOrder3(blueTransformedCoeffs, coeffs[2], modelRotationInv);

		for(int i = 0; i < 9; ++i)
			(*lightingShCoeffs)[i] = Vector3(redTransformedCoeffs[i], greenTransformedCoeffs[i], blueTransformedCoeffs[i]);
		mCallee->Execute();

#if 0
		// Debug:
		Vector3 corners[4];
		bool isOuterCell = false;
		mInterpolation.GetTetrahedronCorners(mTetrahedronIndex, corners, isOuterCell);
		Vector4 weights = mInterpolation.GetCornerWeights(mTetrahedronIndex, position);

		const Vector3 color = isOuterCell ? Vector3(1, 0, 0) : Vector3(0, 0, 1);
		mDebugMesh.DrawLine(corners[0], corners[1], color);
		mDebugMesh.DrawLine(corners[0], corners[2], color);
		mDebugMesh.DrawLine(corners[1], corners[2], color);

		if(!isOuterCell)
		{
			mDebugMesh.DrawLine(corners[0], corners[3], color);
			mDebugMesh.DrawLine(corners[1], corners[3], color);
			mDebugMesh.DrawLine(corners[2], corners[3], color);
		}

		for(int i = 0; i < (isOuterCell ? 3 : 4); i++)
		{
			std::ostringstream oss;
			oss << weights[i];
			mDebugMesh.DrawText(oss.str(), corners[i]);
		}

		**modelMatrix = Matrix4::Identity();
		mDebugMesh.Execute();
#endif
	}
	else
		mCallee->Execute();
}

}
}
