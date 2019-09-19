/*	ApplyTextures.cpp
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "ApplyTextures.h"
#include <molecular/gfx/TextureManager.h>

namespace molecular
{
namespace Gfx
{

void ApplyTextures::Execute()
{
	SetTextureUniforms(mTextures.begin());
}

void ApplyTextures::SetTexture(Hash variable, Hash file)
{
	mTextures[variable] = mTextureManager.GetAsset(file);
}

void ApplyTextures::SetParameter(Hash variable, RenderCmdSink::Texture::Parameter param, RenderCmdSink::Texture::ParamValue value)
{
	auto it = mTextures.find(variable);
	if(it != mTextures.end())
	{
		it->second->GetAsset()->SetParameter(param, value);
	}
}

void ApplyTextures::SetTextureUniforms(TextureMap::const_iterator it)
{
	if(it == mTextures.end())
		mCallee->Execute();
	else
	{
		Binding<Uniform<RenderCmdSink::Texture*> > texture(it->first, this);
		**texture = it->second->Use();
		it++;
		SetTextureUniforms(it);
	}
}

}
}
