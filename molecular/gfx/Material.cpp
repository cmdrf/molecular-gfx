/*	Material.cpp

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

#include "Material.h"

namespace molecular
{
namespace gfx
{

Material::~Material()
{

}

void Material::SetTexture(Hash hash, Hash textureFileName)
{
	// TODO
/*
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
*/
}

void Material::Bind(Scope& scope, int lodLevel)
{
	assert(mNames.size() == mUniforms.size());
	for(size_t i = 0; i < mNames.size(); ++i)
	{
		mUniforms[i]->Apply(scope, mNames[i]);
	}
	// TODO
/*
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
*/
}

int Material::FindEntry(Hash name)
{
	for(size_t i = 0; i < mNames.size(); ++i)
	{
		if(mNames[i] == name)
			return i;
	}
	return -1;
}

}
}
