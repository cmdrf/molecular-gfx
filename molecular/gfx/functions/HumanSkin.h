/*	HumanSkin.h
	Copyright 2013-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef HUMANSKIN_H
#define HUMANSKIN_H

#include <molecular/gfx/RenderFunction.h>
#include "DrawMeshData.h"

namespace molecular
{
namespace Gfx
{

class HumanSkin : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	HumanSkin(TRenderManager& manager, unsigned int dimensions = 1024);

	void Execute();

	void SetStretchTexture(const Hash file){mStretchTexture = mTextureManager.GetAsset(file);}

private:
	class BlurStage : NonCopyable
	{
	public:
		BlurStage(HumanSkin& parent, unsigned int dimensions);
		~BlurStage();

		void Bind();

		RenderCmdSink::Texture* GetTexture() {return mTexture;}

	private:
		RenderCmdSink::Texture* mTexture;
		RenderCmdSink::RenderTarget* mRenderTarget;
		HumanSkin& mParent;
	};

	void Blur(BlurStage& from, BlurStage& to, bool y, float width, bool noBind = false);

	BlurStage mStageTmp, mStage0, mStage1, mStage2, mStage3, mStage4;

	TextureManager& mTextureManager;
	TextureManager::Asset* mStretchTexture;
	DrawMeshData mDrawQuad;
	RenderCmdSink::RenderTarget* mOldRenderTarget;
};

template<class TRenderManager>
HumanSkin::HumanSkin(TRenderManager& manager, unsigned int dimensions) :
	SingleCalleeRenderFunction(manager),
	mStageTmp(*this, dimensions),
	mStage0(*this, dimensions),
	mStage1(*this, dimensions),
	mStage2(*this, dimensions),
	mStage3(*this, dimensions),
	mStage4(*this, dimensions),
	mTextureManager(manager.GetTextureManager()),
	mStretchTexture(nullptr),
	mDrawQuad(manager),
	mOldRenderTarget(nullptr)
{
	// TODO:
//	QuadMeshDataSource quadSource;
//	mDrawQuad.Load(quadSource);
}

}
}

#endif // HUMANSKIN_H
