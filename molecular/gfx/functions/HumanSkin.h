/*	HumanSkin.h

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

#ifndef MOLECULAR_HUMANSKIN_H
#define MOLECULAR_HUMANSKIN_H

#include <molecular/gfx/RenderFunction.h>
#include "DrawMeshData.h"

namespace molecular
{
namespace gfx
{

/// Implementation of subsurface scattering as presented in GPU Gems 3
/** https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch14.html */
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

#endif // MOLECULAR_HUMANSKIN_H
