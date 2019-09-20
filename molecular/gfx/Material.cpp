/*	Material.cpp
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "Material.h"

namespace molecular
{
namespace gfx
{

Material::~Material()
{
	assert(!mBound);
}

void Material::SetTexture(Hash hash, Hash textureFileName)
{
	assert(!mBound);
	auto asset = mTextureManager.GetAsset(textureFileName);
	RenderCmdSink::Texture* texture = asset->GetAsset();
//	texture->SetParameter(Renderer::Texture::kMinFilter, Renderer::Texture::kLinear);
//	texture->SetParameter(Renderer::Texture::kMagFilter, Renderer::Texture::kLinear);
	using TextureBinding = gfx::Scoping::ManualBinding<Uniform<RenderCmdSink::Texture*>>;
	int index = FindEntry(hash);
	if(index == -1)
	{
		TextureBinding* binding = new TextureBinding(hash, mScoping);
		***binding = texture;
		mUniforms.emplace_back(hash, binding, asset);
	}
	else
	{
		***static_cast<TextureBinding*>(mUniforms[index].binding.get()) = texture;
	}
}

void Material::Bind(int lodLevel)
{
	assert(this);
	assert(!mBound);
	for(auto& uniform: mUniforms)
	{
		assert(uniform.binding);
		uniform.binding->Bind();
		if(uniform.texture)
		{
			uniform.texture->Use(lodLevel);
		}
	}
	mBound = true;
}

void Material::Unbind()
{
	assert(mBound);
	for(auto& uniform: mUniforms)
	{
		assert(uniform.binding);
		uniform.binding->Unbind();
	}
	mBound = false;
}

int Material::FindEntry(Hash name)
{
	for(size_t i = 0; i < mUniforms.size(); ++i)
	{
		if(mUniforms[i].name == name)
			return i;
	}
	return -1;
}

}
}
