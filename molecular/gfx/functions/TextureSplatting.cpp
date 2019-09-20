/*	TextureSplatting.cpp
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "TextureSplatting.h"
#include <molecular/gfx/TextureSplattingData.h>

namespace molecular
{
namespace gfx
{

void TextureSplatting::Execute()
{
	Binding<Uniform<RenderCmdSink::Texture*, kMaxTextures> > splattingTexturesArray("splattingTexturesArray"_H, this);
	Binding<Uniform<float> > splattingScale("splattingScale"_H, this);

	for(unsigned int i = 0; i < kMaxTextures; ++i)
	{
		if(mTextures[i])
			(*splattingTexturesArray)[i] = mTextures[i]->Use();
		else
			(*splattingTexturesArray)[i] = nullptr;
	}
	**splattingScale = mTextureScale;

	if(mSplattingMaps[0].texture)
	{
		if(mSplattingMaps[0].components == 3)
		{
			Binding<Uniform<RenderCmdSink::Texture*>> splattingMapRgb0("splattingMapRgb0"_H, this);
			**splattingMapRgb0 = mSplattingMaps[0].texture;
			BindMap1();
		}
		else if(mSplattingMaps[0].components == 4)
		{
			Binding<Uniform<RenderCmdSink::Texture*>> splattingMapRgba0("splattingMapRgba0"_H, this);
			**splattingMapRgba0 = mSplattingMaps[0].texture;
			BindMap1();
		}
	}
}

void TextureSplatting::SetSplattingMap(unsigned int index, RenderCmdSink::Texture* map, unsigned int components)
{
	assert(index < kMaxSplattingMaps);
	assert(components == 3 || components == 4);
	SplattingMap splattingMap = {map, components};
	mSplattingMaps[index] = splattingMap;
}

void TextureSplatting::SetTexture(unsigned int index, Hash texture)
{
	assert(index < kMaxTextures);
	mTextures[index] = mTextureManager.GetAsset(texture);
}

void TextureSplatting::BindMap1()
{
	if(mSplattingMaps[1].texture)
	{
		if(mSplattingMaps[1].components == 3)
		{
			Binding<Uniform<RenderCmdSink::Texture*>> splattingMapRgb1("splattingMapRgb1"_H, this);
			**splattingMapRgb1 = mSplattingMaps[1].texture;
			mCallee->Execute();
		}
		else if(mSplattingMaps[1].components == 4)
		{
			Binding<Uniform<RenderCmdSink::Texture*>> splattingMapRgba1("splattingMapRgba1"_H, this);
			**splattingMapRgba1 = mSplattingMaps[1].texture;
			mCallee->Execute();
		}
	}
	else
		mCallee->Execute();
}

}
}
