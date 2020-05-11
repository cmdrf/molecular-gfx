/*	CascadedShadowMapping.h

MIT License

Copyright (c) 2020 Fabian Herb

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

#ifndef MOLECULAR_GFX_CASCADEDSHADOWMAPPING_H
#define MOLECULAR_GFX_CASCADEDSHADOWMAPPING_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace gfx
{

using namespace util;

class CascadedShadowMapping : public SingleCalleeRenderFunction
{
public:	
	template<class TRenderManager>
	CascadedShadowMapping(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager)
	{
		SetupShadowResources();
	}

	~CascadedShadowMapping() override;


	void SetCascadeSplitLambda(float cascadeSplitLambda) {mCascadeSplitLambda = cascadeSplitLambda;}
	void SetShadowDrawingDistance(float shadowDrawingDistance) {mShadowDrawingDistance = shadowDrawingDistance;}

protected:
	void HandleExecute(Scope& scope) override;

private:
	/// Matrices for each cascade calculated by CalculateCascades()
	struct CascadesInfo
	{
		Matrix4 globalShadowProj[3];
		Matrix4 globalShadowViews[3];

		/// Stores the cascade splits in world coordinates
		Eigen::Vector3f cascadeSplit;
	};

	void SetupShadowResources();
	void TeardownShadowResources();

	/// Calculates projection matrix, view matrix and split distance for each cascade
	CascadesInfo CalculateCascades(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Vector3& lightDirection);

	static const int NUM_SHADOW_MAPS = 3;

	RenderCmdSink::Texture* mShadowMaps[NUM_SHADOW_MAPS] = {nullptr};
	RenderCmdSink::RenderTarget* mRenderTargets[NUM_SHADOW_MAPS] = {nullptr};

	float mCascadeSplitLambda = 0.75f;

	/// Defines max distance that is still shadowed
	float mShadowDrawingDistance = 200;

	unsigned int mShadowWidth = 2048;
	unsigned int mShadowHeight = 2048;

	/// Transforms Normalized Device Coordinates (-1 to 1) to UVW (0 to 1) coordinates
	static const Matrix4 kBiasMatrix;
};

}
}

#endif
