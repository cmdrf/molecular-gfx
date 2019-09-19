/*	ShadowMapping.cpp
	Copyright 2012-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "ShadowMapping.h"
#include "util/Matrix3.h"
#include "util/Matrix4.h"
#include "gfx/TrapezoidalShadowMapping.h"

namespace Gfx
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

void ShadowMapping::Execute()
{
	Binding<Output> glPosition("gl_Position"_H, this);
	Binding<Uniform<Matrix4> > shadowViewProjMatrix0("shadowViewProjMatrix0"_H, this);

	{
		Binding<Uniform<Matrix4> > viewMatrix("viewMatrix"_H, this);
		Binding<Uniform<Matrix4> > projMatrix("projectionMatrix"_H, this);
		Matrix4 viewProjMatrix = **projMatrix * **viewMatrix;
		Matrix4 viewProjMatrixInverse = viewProjMatrix.Inverse();
		Vector4 nearHom = viewProjMatrixInverse * Vector4(0, 0, -1, 1);
		Vector3 nearCenter = nearHom.Xyz() / nearHom[3];

		if(const Uniform<Vector3>* lightDirection0 = GetVariable<Uniform<Vector3>>("lightDirection0"_H))
		{
			Matrix4 lightProjection = Matrix4::ProjectionOrthographic(20, 20, -10, 10);
			Matrix4 lightView = Matrix4(Matrix3::LookAtZ(**lightDirection0 * -1).Transposed()) * Matrix4::Translation(nearCenter * -1.0);
			Matrix4 warp = TrapezoidalShadowMapping::LightProjection(viewProjMatrix, lightProjection * lightView);
			**projMatrix = warp * lightProjection;
			**viewMatrix = lightView;
		}
		else // Point or spot light
		{

		}
#if SHADOW_TEST
		Binding<Output> fragmentColor("fragmentColor"_H, this);
#else
		**shadowViewProjMatrix0 =  Matrix4(kBiasMatrix) * **projMatrix * **viewMatrix;

		RenderCmdSink::RenderTarget* oldTarget = mRenderer.GetTarget();
		mRenderer.SetTarget(mRenderTarget);
		mRenderer.Clear(false, true);
		mRenderer.SetRasterizationState(false, RenderCmdSink::kFront);
#endif
		if(mCallee)
			mCallee->Execute();

#if !SHADOW_TEST
		mRenderer.SetTarget(oldTarget);
	}
	// Render scene normally
	Binding<Uniform<RenderCmdSink::Texture*> > shadowTexture0("shadowTexture0"_H, this);
	Binding<Output> fragmentColor("fragmentColor"_H, this);

	**shadowTexture0 = mShadowMap;
	mRenderer.SetRasterizationState(false, RenderCmdSink::kBack);
	if(mCallee)
		mCallee->Execute();
#else
	}
#endif
}

}

#undef SHADOW_TEST
