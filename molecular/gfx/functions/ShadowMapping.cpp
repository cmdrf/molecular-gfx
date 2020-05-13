/*	ShadowMapping.cpp

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#include "ShadowMapping.h"
#include <molecular/util/Matrix3.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace gfx
{

const float ShadowMapping::kBiasMatrix[16] = {
	0.5, 0.0, 0.0, 0.5,
	0.0, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.5,
	0.0, 0.0, 0.0, 1.0
};

ShadowMapping::~ShadowMapping()
{
	if(mRenderTarget)
		mRenderer.DestroyRenderTarget(mRenderTarget);
	if(mShadowMap)
		mRenderer.DestroyTexture(mShadowMap);
}

#define SHADOW_TEST 0

void ShadowMapping::HandleExecute(Scope& scope)
{
	scope.Set("gl_Position"_H, Output());
	Uniform<Matrix4>& shadowViewProjMatrix0 = scope.Bind<Uniform<Matrix4>>("shadowViewProjMatrix0"_H);

	{
		Scope shadowMappingScope(scope);
		Uniform<Matrix4>& viewMatrix = shadowMappingScope.Bind<Uniform<Matrix4>>("viewMatrix"_H);
		Uniform<Matrix4>& projMatrix = shadowMappingScope.Bind<Uniform<Matrix4>>("projectionMatrix"_H);

		if(scope.Has("lightDirection0"_H))
		{
			const Vector3 lightDirection0 = *scope.Get<Uniform<Vector3>>("lightDirection0"_H);
			*viewMatrix = Matrix4(Matrix3::LookAtZ(lightDirection0 * -1).Transposed());
			*projMatrix = Matrix4::ProjectionOrthographic(10, 10, -5, 5);
		}
		else // Point or spot light
		{

		}
#if SHADOW_TEST
		Binding<Output> fragmentColor("fragmentColor"_H, this);
#else
		*shadowViewProjMatrix0 =  Matrix4(kBiasMatrix) * *projMatrix * *viewMatrix;

		RenderCmdSink::RenderTarget* oldTarget = mRenderer.GetTarget();
		mRenderer.SetTarget(mRenderTarget);
		mRenderer.Clear(false, true);
		mRenderer.SetRasterizationState(false, RenderCmdSink::kFront);
#endif
		if(mCallee)
			mCallee->Execute(scope);

#if !SHADOW_TEST
		mRenderer.SetTarget(oldTarget);
	}
	// Render scene normally
	scope.Set<Uniform<RenderCmdSink::Texture*>>("shadowTexture0"_H, mShadowMap);
	scope.Set("fragmentColor"_H, Output());

	mRenderer.SetRasterizationState(false, RenderCmdSink::kBack);
	if(mCallee)
		mCallee->Execute(scope);
#else
	}
#endif
}

}
}
#undef SHADOW_TEST
