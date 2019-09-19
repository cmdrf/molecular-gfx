/*	ApplyTextures.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef APPLYTEXTURES_H
#define APPLYTEXTURES_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/TextureManager.h>

namespace molecular
{
namespace Gfx
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

#endif // APPLYTEXTURES_H
