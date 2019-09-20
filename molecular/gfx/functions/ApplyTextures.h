/*	ApplyTextures.h

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

#ifndef MOLECULAR_APPLYTEXTURES_H
#define MOLECULAR_APPLYTEXTURES_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/TextureManager.h>

namespace molecular
{
namespace gfx
{

/// Set texture uniforms in subfunctions
class ApplyTextures : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	ApplyTextures(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mTextureManager(manager.GetTextureManager())
	{}

	void Execute() override;

	void SetTexture(Hash variable, Hash file);

	void SetParameter(Hash variable, RenderCmdSink::Texture::Parameter param, RenderCmdSink::Texture::ParamValue value);

private:
	typedef std::map<Hash, TextureManager::Asset*> TextureMap;

	void SetTextureUniforms(TextureMap::const_iterator it);

	TextureManager mTextureManager;
	TextureMap mTextures;
};

}
}

#endif // MOLECULAR_APPLYTEXTURES_H
