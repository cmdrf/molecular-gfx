/*	HumanSkin.cpp

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

#include "HumanSkin.h"
#include <molecular/gfx/DefaultProgramData.h>

namespace molecular
{
namespace gfx
{

void HumanSkin::HandleExecute(Scope& scope)
{
	if(scope.Has("shadowTexture0"_H))
	{
		mRenderer.SetDepthState(false, false);

		// Texture space render
		Scope blurScope(&scope);
		blurScope.Unset("projectionMatrix"_H);
		mOldRenderTarget = mRenderer.GetTarget();
		mStage4.Bind();
		mCallee->Execute(&blurScope);

		Blur(mStage4, mStageTmp, false, 2.0f, blurScope);
		Blur(mStageTmp, mStage0, true, 2.0f, blurScope);
		Blur(mStage0, mStageTmp, false, 3.387f, blurScope);
		Blur(mStageTmp, mStage1, true, 3.387f, blurScope);
		Blur(mStage1, mStageTmp, false, 5.60683f, blurScope);
		Blur(mStageTmp, mStage2, true, 5.60683f, blurScope);
		Blur(mStage2, mStageTmp, false, 10.8565f, blurScope);
		Blur(mStageTmp, mStage3, true, 10.8565f, blurScope);
		Blur(mStage3, mStageTmp, false, 21.1558f, blurScope);
		Blur(mStageTmp, mStage4, true, 21.1558f, blurScope);

		mRenderer.SetTarget(mOldRenderTarget);

		mRenderer.SetDepthState(true, true);

		// Final render
		scope.Set<Uniform<RenderCmdSink::Texture*>>("blur2Texture"_H, mStage0.GetTexture());
		scope.Set<Uniform<RenderCmdSink::Texture*>>("blur4Texture"_H, mStage1.GetTexture());
		scope.Set<Uniform<RenderCmdSink::Texture*>>("blur8Texture"_H, mStage2.GetTexture());
		scope.Set<Uniform<RenderCmdSink::Texture*>>("blur16Texture"_H, mStage3.GetTexture());
		scope.Set<Uniform<RenderCmdSink::Texture*>>("blur32Texture"_H, mStage4.GetTexture());

		scope.Set("noBlurWeight"_H, Uniform<Vector3>(Vector3(0.651023f, 0.761059f, 0.661339f)));
		scope.Set("blur2Weight"_H, Uniform<Vector3>(Vector3(0.278155f, 0.560714f, 0.350606f)));
		scope.Set("blur4Weight"_H, Uniform<Vector3>(Vector3(0.32887f, 0.32887f, 0.0f)));
		scope.Set("blur8Weight"_H, Uniform<Vector3>(Vector3(0.31438f, 0.0f, 0.0f)));
		scope.Set("blur16Weight"_H, Uniform<Vector3>(Vector3(1.0f, 0.00618938f, 0.00618938f)));
		scope.Set("blur32Weight"_H, Uniform<Vector3>(Vector3(0.220194f, 0.0f, 0.0f)));

		mCallee->Execute(&scope);
	}
	else
	{
		// Texture map render
		mCallee->Execute(&scope);
	}
}

HumanSkin::BlurStage::BlurStage(HumanSkin& parent, unsigned int dimensions) :
	mParent(parent)
{
	mTexture = mParent.mRenderer.CreateTexture();
	mTexture->Store(dimensions, dimensions, nullptr, PF_B8G8R8A8);
	mTexture->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinear);
	mTexture->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);
	mTexture->SetParameter(RenderCmdSink::Texture::kWrapS, RenderCmdSink::Texture::kClampToEdge);
	mTexture->SetParameter(RenderCmdSink::Texture::kWrapT, RenderCmdSink::Texture::kClampToEdge);

	mRenderTarget = mParent.mRenderer.CreateRenderTarget();
	mRenderTarget->AttachColorBuffer(mTexture);
	mRenderTarget->AttachDepthBuffer(dimensions, dimensions, PF_DEPTH_16);
}

HumanSkin::BlurStage::~BlurStage()
{
	mParent.mRenderer.DestroyRenderTarget(mRenderTarget);
	mParent.mRenderer.DestroyTexture(mTexture);
}

void HumanSkin::BlurStage::Bind()
{
	mParent.mRenderer.SetTarget(mRenderTarget);
}

void HumanSkin::Blur(BlurStage& from, BlurStage& to, bool y, float width, Scope& scope)
{
	Hash gaussWidthVar = y ? "gaussWidthY"_H : "gaussWidthX"_H;

	scope.Set<Uniform<RenderCmdSink::Texture*>>("blurSourceTexture"_H, from.GetTexture());
	if(mStretchTexture)
		scope.Set<Uniform<RenderCmdSink::Texture*>>("stretchTexture"_H, mStretchTexture->Use());
	scope.Set(gaussWidthVar, Uniform<float>(width));

	to.Bind();
	mDrawQuad.Execute(&scope);
}

}
}
