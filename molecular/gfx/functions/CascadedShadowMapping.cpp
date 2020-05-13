/*	CascadedShadowMapping.cpp

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

#include "CascadedShadowMapping.h"

#include <array>

namespace molecular
{
namespace gfx
{

using namespace util;

const Matrix4 CascadedShadowMapping::kBiasMatrix{
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0
};

namespace
{

Eigen::Matrix4f LookAtMatrix(Eigen::Vector3f eye, Eigen::Vector3f direction, Eigen::Vector3f up)
{
	Eigen::Vector3f x = up.cross(direction);
	x.normalize();
	up = direction.cross(x);
	up.normalize();

	Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
	mat.col(0).head(3) << x;
	mat.col(1).head(3) << up;
	mat.col(2).head(3) << -direction;

	mat.topLeftCorner<3, 3>().transposeInPlace();

	mat.col(3).head(3) << -x.dot(eye), -up.dot(eye), direction.dot(eye);

	return mat;
}

Matrix4 ProjectionOrthographicBox(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 outMatrix = Matrix4::Identity();

	outMatrix[0][0] = 2.0f / (right - left);
	outMatrix[1][1] = 2.0f / (top - bottom);
	outMatrix[2][2] = - 2.0f / (zFar - zNear);
	outMatrix[0][3] = - (right + left) / (right - left);
	outMatrix[1][3] = - (top + bottom) / (top - bottom);
	outMatrix[2][3] = - (zFar + zNear) / (zFar - zNear);

	return outMatrix;
}

std::array<float, 2> NearFarFromProjectionMatrix(const Matrix4& projectionMatrix)
{
	// https://dougrogers.blogspot.com/2013/02/how-to-derive-near-and-far-clip-plane.html
	const float C = projectionMatrix[2][2];
	const float D = projectionMatrix[2][3];

	const float n = D / (C - 1.0);
	const float f = D / (C + 1.0);
	return std::array<float, 2>{n, f};
}

}

CascadedShadowMapping::~CascadedShadowMapping()
{
	TeardownShadowResources();
}

void CascadedShadowMapping::HandleExecute(Scope& scope)
{
	if(!mCallee)
		return;

	scope.Set("gl_Position"_H, Output());

	CascadesInfo cascades;
	{
		Scope shadowMappingScope(scope);
		Uniform<Matrix4>& viewMatrix = shadowMappingScope.Bind<Uniform<Matrix4>>("viewMatrix"_H);
		Uniform<Matrix4>& projMatrix = shadowMappingScope.Bind<Uniform<Matrix4>>("projectionMatrix"_H);
		const Vector3 lightDirection0 = *shadowMappingScope.Get<Uniform<Vector3>>("lightDirection0"_H);

		/* Unbind fragmentColor if previously bound by ViewSetup, since we don't need to generate
			color information: */
		shadowMappingScope.Unset("fragmentColor"_H);

		cascades = CalculateCascades(*projMatrix, *viewMatrix, lightDirection0);

		RenderCmdSink::RenderTarget* oldTarget = mRenderer.GetTarget();
		mRenderer.SetRasterizationState(false, RenderCmdSink::kFront);

		for(unsigned i = 0; i < NUM_SHADOW_MAPS; i++)
		{
			*projMatrix = cascades.globalShadowProj[i];
			*viewMatrix = cascades.globalShadowViews[i];

		   mRenderer.SetTarget(mRenderTargets[i]);
		   mRenderer.Clear(false, true);

		   mCallee->Execute(shadowMappingScope);
		}

		mRenderer.SetTarget(oldTarget);
	}

	// Render scene normally
	Uniform<RenderCmdSink::Texture*, 3>& shadowTextures = scope.Bind<Uniform<RenderCmdSink::Texture*, 3>>("shadowTextures"_H);
	Uniform<Matrix4, 3>& shadowViewProjMatrices = scope.Bind<Uniform<Matrix4, 3>>("shadowViewProjMatrices"_H);
	Uniform<float, 3>& shadowCascadeSplits = scope.Bind<Uniform<float, 3>>("shadowCascadeSplits"_H);
	scope.Set("fragmentColor"_H, Output());

	for(int i = 0; i < 3; ++i)
	{
		shadowTextures[i] = mShadowMaps[i];
		shadowViewProjMatrices[i] =  kBiasMatrix * cascades.globalShadowProj[i] * cascades.globalShadowViews[i];
		shadowCascadeSplits[i] = cascades.cascadeSplit[i];
	}

	mRenderer.SetRasterizationState(false, RenderCmdSink::kBack);
	mCallee->Execute(scope);
}

void CascadedShadowMapping::SetupShadowResources()
{
	for(int i = 0; i < NUM_SHADOW_MAPS; i++)
	{
		mShadowMaps[i] = mRenderer.CreateTexture();
		mShadowMaps[i]->Store(mShadowWidth, mShadowHeight, nullptr, PF_DEPTH_24);
		mShadowMaps[i]->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinear);
		mShadowMaps[i]->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);
		mShadowMaps[i]->SetParameter(RenderCmdSink::Texture::kCompareFunc, RenderCmdSink::Texture::kLessEqual);
		mShadowMaps[i]->SetParameter(RenderCmdSink::Texture::kCompareMode, RenderCmdSink::Texture::kCompareRefToTexture);
		mShadowMaps[i]->SetParameter(RenderCmdSink::Texture::kWrapS, RenderCmdSink::Texture::kClampToEdge);
		mShadowMaps[i]->SetParameter(RenderCmdSink::Texture::kWrapT, RenderCmdSink::Texture::kClampToEdge);

		mRenderTargets[i] = mRenderer.CreateRenderTarget();
		mRenderTargets[i]->AttachDepthBuffer(mShadowMaps[i]);
	}
}

void CascadedShadowMapping::TeardownShadowResources()
{
	for(int i = 0; i < NUM_SHADOW_MAPS; i++)
	{
		if(mRenderTargets[i])
			mRenderer.DestroyRenderTarget(mRenderTargets[i]);
		if(mShadowMaps[i])
			mRenderer.DestroyTexture(mShadowMaps[i]);
	}
}

CascadedShadowMapping::CascadesInfo CascadedShadowMapping::CalculateCascades(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Vector3& lightDirection)
{
	const auto nearFar = NearFarFromProjectionMatrix(projectionMatrix);
	const float nearClip = nearFar[0];
	const float clipRange = nearFar[1] - nearClip;

	const float minZ = nearClip;
	const float maxZ = mShadowDrawingDistance;

	const float range = maxZ - minZ;
	const float ratio = maxZ / minZ;

	CascadesInfo out;

	// Calculate split depths based on view camera furstum
	// Based on method presentd in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
	for (int cascade = 0; cascade < NUM_SHADOW_MAPS; cascade++)
	{
		const float p = (cascade + 1) / static_cast<float>(NUM_SHADOW_MAPS);
		const float log = minZ * std::pow(ratio, p);
		const float uniform = minZ + range * p;
		const float d = mCascadeSplitLambda * (log - uniform) + uniform;
		const float splitDist = (d - nearClip) / clipRange;

		Eigen::Vector3f frustumCorners[8] = {
			Eigen::Vector3f(-1.0f,  1.0f, -1.0f),
			Eigen::Vector3f( 1.0f,  1.0f, -1.0f),
			Eigen::Vector3f( 1.0f, -1.0f, -1.0f),
			Eigen::Vector3f(-1.0f, -1.0f, -1.0f),
			Eigen::Vector3f(-1.0f,  1.0f,  1.0f),
			Eigen::Vector3f( 1.0f,  1.0f,  1.0f),
			Eigen::Vector3f( 1.0f, -1.0f,  1.0f),
			Eigen::Vector3f(-1.0f, -1.0f,  1.0f),
		};

		// Project frustum corners into world space
		Eigen::Matrix4f invCam = (projectionMatrix * viewMatrix).Inverse().ToEigen();

		for (int i = 0; i < 8; i++)
		{
			Eigen::Vector4f currentCorner;
			currentCorner << frustumCorners[i], 1.0f;
			Eigen::Vector4f invCorner = invCam * currentCorner;
			frustumCorners[i] = invCorner.head(3) / invCorner(3);
		}

		for (int i = 0; i < 4; i++)
		{
			const Eigen::Vector3f dist = frustumCorners[i + 4] - frustumCorners[i];
			frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
		}

		// Get frustum center
		Eigen::Vector3f frustumCenter = Eigen::Vector3f::Zero();
		for (int i = 0; i < 8; i++)
			frustumCenter += frustumCorners[i];

		frustumCenter /= 8.0f;

		float radius = 0.0f;
		for (int i = 0; i < 8; i++)
		{
			float distance = (frustumCorners[i] - frustumCenter).norm();
			radius = std::max(radius, distance);
		}
		radius = std::ceil(radius * 16.0f) / 16.0f;

		const Vector3 maxExtents(radius, radius, radius);
		const Vector3 minExtents = -maxExtents;

		const Eigen::Vector3f lightDir{lightDirection.X(), lightDirection.Y(), lightDirection.Z()};
		const Eigen::Matrix4f lightViewMatrix = LookAtMatrix(frustumCenter + (lightDir * minExtents.Z()), lightDir, Eigen::Vector3f(0.0f, 0.0f, 1.0f));

		// Store split distance and matrix in cascade
		out.cascadeSplit(cascade) = (nearClip + splitDist * clipRange) * -1.0f;
		out.globalShadowProj[cascade] = ProjectionOrthographicBox(minExtents.X(), maxExtents.X(), minExtents.Y(), maxExtents.Y(), 0.0f, maxExtents.Z() - minExtents.Z());
		out.globalShadowViews[cascade] = Matrix4(lightViewMatrix);
	}
	return out;
}

}
}
