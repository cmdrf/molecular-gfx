/*	TextureSplatting.h

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

#ifndef MOLECULAR_TEXTURESPLATTING_H
#define MOLECULAR_TEXTURESPLATTING_H

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

#endif // MOLECULAR_TEXTURESPLATTING_H
