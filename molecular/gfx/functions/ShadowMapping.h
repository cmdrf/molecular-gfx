/*	ShadowMapping.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef SHADOWMAPPING_H
#define SHADOWMAPPING_H

#include "gfx/RenderFunction.h"

namespace Gfx
{

class ShadowMapping : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	ShadowMapping(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mShadowMap(nullptr),
		mRenderTarget(nullptr)
	{
		mShadowMap = mRenderer.CreateTexture();
		mShadowMap->Store(1024, 1024, nullptr, PF_DEPTH_24);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinear);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kCompareFunc, RenderCmdSink::Texture::kLessEqual);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kCompareMode, RenderCmdSink::Texture::kCompareRefToTexture);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kWrapS, RenderCmdSink::Texture::kClampToEdge);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kWrapT, RenderCmdSink::Texture::kClampToEdge);

		mRenderTarget = mRenderer.CreateRenderTarget();
		mRenderTarget->AttachDepthBuffer(mShadowMap);
	}

	~ShadowMapping();

	void Execute() override;

private:
	RenderCmdSink::Texture* mShadowMap;
	RenderCmdSink::RenderTarget* mRenderTarget;

	/// Transforms Normalized Device Coordinates (-1 to 1) to UVW (0 to 1) coordinates
	static const float kBiasMatrix[16];
};

}

#endif // SHADOWMAPPING_H
