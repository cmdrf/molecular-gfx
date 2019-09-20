/*	TextureSplatting.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TEXTURESPLATTING_H
#define TEXTURESPLATTING_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/TextureManager.h>

namespace molecular
{
namespace gfx
{

class TextureSplatting : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	TextureSplatting(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mTextureManager(manager.GetTextureManager()),
		mTextureScale(100)
	{
		for(unsigned int i = 0; i < kMaxTextures; ++i)
			mTextures[i] = nullptr;

		for(unsigned int i = 0; i < kMaxSplattingMaps; ++i)
			mSplattingMaps[i].texture = nullptr;
	}
	void Execute() override;

	void SetTextureScale(float scale) {mTextureScale = scale;}

	void SetSplattingMap(unsigned int index, RenderCmdSink::Texture* map, unsigned int components = 4);
	RenderCmdSink::Texture* GetSplattingMapTexture(unsigned int index) {return mSplattingMaps[index].texture;}
	unsigned int GetSplattingMapComponents(unsigned int index) {return mSplattingMaps[index].components;}
	void SetTexture(unsigned int index, Hash texture);

	static const unsigned int kMaxSplattingMaps = 2;
	static const unsigned int kMaxTextures = kMaxSplattingMaps * 4;

private:
	void BindMap1();

	struct SplattingMap
	{
		RenderCmdSink::Texture* texture;
		unsigned int components;
	};

	TextureManager& mTextureManager;
	SplattingMap mSplattingMaps[kMaxSplattingMaps];
	TextureManager::Asset* mTextures[kMaxTextures];
	float mTextureScale;
};

}
}

#endif // TEXTURESPLATTING_H
