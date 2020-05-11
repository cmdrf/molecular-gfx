/*	TextureSplatting.cpp

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#include "TextureSplatting.h"
#include <molecular/gfx/TextureSplattingData.h>

namespace molecular
{
namespace gfx
{

void TextureSplatting::HandleExecute(Scope& scope)
{
	auto& splattingTexturesArray = scope.Bind<Uniform<RenderCmdSink::Texture*, kMaxTextures>>("splattingTexturesArray"_H);
	scope.Set<Uniform<float>>("splattingScale"_H, mTextureScale);

	for(unsigned int i = 0; i < kMaxTextures; ++i)
	{
		if(mTextures[i])
			splattingTexturesArray[i] = mTextures[i]->Use();
		else
			splattingTexturesArray[i] = nullptr;
	}

	if(mSplattingMaps[0].texture)
	{
		if(mSplattingMaps[0].components == 3)
			scope.Set<Uniform<RenderCmdSink::Texture*>>("splattingMapRgb0"_H, mSplattingMaps[0].texture);
		else if(mSplattingMaps[0].components == 4)
			scope.Set<Uniform<RenderCmdSink::Texture*>>("splattingMapRgba0"_H, mSplattingMaps[0].texture);
	}
	if(mSplattingMaps[1].texture)
	{
		if(mSplattingMaps[1].components == 3)
			scope.Set<Uniform<RenderCmdSink::Texture*>>("splattingMapRgb1"_H, mSplattingMaps[1].texture);
		else if(mSplattingMaps[1].components == 4)
			scope.Set<Uniform<RenderCmdSink::Texture*>>("splattingMapRgba1"_H, mSplattingMaps[1].texture);
	}
	mCallee->Execute(&scope);
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

}
}
