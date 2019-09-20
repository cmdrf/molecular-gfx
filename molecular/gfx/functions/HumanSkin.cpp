/*	HumanSkin.cpp
	Copyright 2013-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "HumanSkin.h"
#include <molecular/gfx/DefaultProgramData.h>

namespace molecular
{
namespace Gfx
{

void HumanSkin::Execute()
{
	SkeletalManualBinding projMatrix("projectionMatrix"_H, this, nullptr);
	if(mScoping["shadowTexture0"_H])
	{
		mRenderer.SetDepthState(false, false);

		// Texture space render
		projMatrix.Bind();
		mOldRenderTarget = mRenderer.GetTarget();
		mStage4.Bind();
		mCallee->Execute();

		Blur(mStage4, mStageTmp, false, 2.0f);
		Blur(mStageTmp, mStage0, true, 2.0f);
		Blur(mStage0, mStageTmp, false, 3.387f);
		Blur(mStageTmp, mStage1, true, 3.387f);
		Blur(mStage1, mStageTmp, false, 5.60683f);
		Blur(mStageTmp, mStage2, true, 5.60683f);
		Blur(mStage2, mStageTmp, false, 10.8565f);
		Blur(mStageTmp, mStage3, true, 10.8565f);
		Blur(mStage3, mStageTmp, false, 21.1558f);
		Blur(mStageTmp, mStage4, true, 21.1558f);

		mRenderer.SetTarget(mOldRenderTarget);

		mRenderer.SetDepthState(true, true);

		projMatrix.Unbind();

		// Final render
		Binding<Uniform<RenderCmdSink::Texture*> > blur2Texture("blur2Texture", this);
		Binding<Uniform<RenderCmdSink::Texture*> > blur4Texture("blur4Texture", this);
		Binding<Uniform<RenderCmdSink::Texture*> > blur8Texture("blur8Texture", this);
		Binding<Uniform<RenderCmdSink::Texture*> > blur16Texture("blur16Texture", this);
		Binding<Uniform<RenderCmdSink::Texture*> > blur32Texture("blur32Texture", this);

		**blur2Texture = mStage0.GetTexture();
		**blur4Texture = mStage1.GetTexture();
		**blur8Texture = mStage2.GetTexture();
		**blur16Texture = mStage3.GetTexture();
		**blur32Texture = mStage4.GetTexture();

		Binding<Uniform<Vector3> > noBlurWeight("noBlurWeight", this);
		Binding<Uniform<Vector3> > blur2Weight("blur2Weight", this);
		Binding<Uniform<Vector3> > blur4Weight("blur4Weight", this);
		Binding<Uniform<Vector3> > blur8Weight("blur8Weight", this);
		Binding<Uniform<Vector3> > blur16Weight("blur16Weight", this);
		Binding<Uniform<Vector3> > blur32Weight("blur32Weight", this);

		**noBlurWeight = Vector3(0.651023f, 0.761059f, 0.661339f);
		**blur2Weight = Vector3(0.278155f, 0.560714f, 0.350606f);
		**blur4Weight = Vector3(0.32887f, 0.32887f, 0.0f);
		**blur8Weight = Vector3(0.31438f, 0.0f, 0.0f);
		**blur16Weight = Vector3(1.0f, 0.00618938f, 0.00618938f);
		**blur32Weight = Vector3(0.220194f, 0.0f, 0.0f);

		mCallee->Execute();
	}
	else
	{
		// Texture map render
		mCallee->Execute();
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

void HumanSkin::Blur(BlurStage& from, BlurStage& to, bool y, float width, bool noBind)
{
	Hash gaussWidthVar = y ? "gaussWidthY"_H : "gaussWidthX"_H;

	Binding<Uniform<RenderCmdSink::Texture*> > sourceTexture("blurSourceTexture", this);
	Binding<Uniform<RenderCmdSink::Texture*> > stretchTexture("stretchTexture", this);
	Binding<Uniform<float> > gaussWidth(gaussWidthVar, this);

	**sourceTexture = from.GetTexture();
	if(mStretchTexture)
		**stretchTexture = mStretchTexture->Use();
	**gaussWidth = width;

	if(!noBind)
		to.Bind();
	mDrawQuad.Execute();
}

}
}
