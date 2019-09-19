/*	SetShCoefficients.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef SETSHCOEFFICIENTS_H
#define SETSHCOEFFICIENTS_H

#include <molecular/gfx/RenderFunction.h>
#include "DrawDebugMesh.h"

namespace molecular
{
namespace Gfx
{

class TetrahedronInterpolation;

/// Sets spherical harmonics coefficients based on tetrahedron interpolation
class SetShCoefficients : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	SetShCoefficients(TRenderManager& renderManager, TetrahedronInterpolation& interpolation) :
		SingleCalleeRenderFunction(renderManager),
//		mDebugMesh(renderManager),
		mInterpolation(interpolation)
	{}

	void Execute() override;

private:
//	DrawDebugMesh mDebugMesh;
	TetrahedronInterpolation& mInterpolation;
	int mTetrahedronIndex = 0;
};

}
}

#endif // SETSHCOEFFICIENTS_H
