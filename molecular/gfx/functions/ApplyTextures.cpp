/*	ApplyTextures.cpp

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

#include "ApplyTextures.h"
#include <molecular/gfx/TextureManager.h>

namespace molecular
{
namespace gfx
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
	{
		if(mCallee)
			mCallee->Execute();
	}
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
