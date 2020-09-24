/*	TextureManager.h

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

#ifndef MOLECULAR_GFX_TEXTUREMANAGER_H
#define MOLECULAR_GFX_TEXTUREMANAGER_H

#include "AssetManager.h"
#include "opengl/PixelFormatConversion.h"
#include "RenderCmdSink.h"

#include <molecular/util/Blob.h>
#include <molecular/util/TaskDispatcher.h>
#include <molecular/util/DdsFile.h>
#include <molecular/util/TgaFile.h>
#include <molecular/util/StringUtils.h>
#include <molecular/util/KtxFile.h>
#include <molecular/util/FileTypeIdentification.h>

#ifdef min
#undef min
#endif

namespace molecular
{
namespace gfx
{

typedef AssetManager<RenderCmdSink::Texture*, 10, true, Hash> TextureManager;

/// Loads textures
/** An instance of this gets passed to the constructor of TextureManager. */
template<class TRenderManager>
class TextureLoader : public TextureManager::Loader
{
public:
	typedef TRenderManager RenderManager;

	explicit TextureLoader(RenderManager& renderManager) : mRenderManager(renderManager) {}

	RenderCmdSink::Texture* Create() override;
	void Destroy(RenderCmdSink::Texture*& asset) override;

	/** @param asset Object to get the texture location from and load texture data to. */
	void StartLoad(TextureManager::Asset& asset, unsigned int minLevel, unsigned int maxLevel) override;

	void Unload(RenderCmdSink::Texture*& asset, unsigned int minLevel, unsigned int maxLevel) override;

private:
	RenderManager& mRenderManager;

	static void StoreTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel);

	static void StoreTgaTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel);

	static void StoreKtxTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel);

	/// Stores loaded DDS texture into video memory
	static void StoreDdsTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel);
};

/*****************************************************************************/

template<class TRenderManager>
RenderCmdSink::Texture* TextureLoader<TRenderManager>::Create()
{
	return mRenderManager.GetRenderCmdSink().CreateTexture();
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::Destroy(RenderCmdSink::Texture*& asset)
{
	mRenderManager.GetRenderCmdSink().DestroyTexture(asset);
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::StartLoad(TextureManager::Asset& asset, unsigned int minLevel, unsigned int maxLevel)
{
	if(maxLevel >= kLodLevels-1)
	{
		// Load all higher levels too
		maxLevel = 9999;
	}

	Hash file = asset.GetLocation();

	try
	{
		auto store = std::bind(StoreTexture, std::ref(asset), std::placeholders::_1, minLevel, maxLevel);
		mRenderManager.GetFileServer().ReadFile(file, store, mRenderManager.GetGlTaskQueue());
	}
	catch(std::exception& e)
	{
		LOG(ERROR) << "Error loading " << file << ": " << e.what();
		asset.SetState(0, TextureManager::Asset::kFailed); // TODO: Set all levels
	}
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::Unload(RenderCmdSink::Texture*& /*asset*/, unsigned int /*minLevel*/, unsigned int /*maxLevel*/)
{
	// TODO
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::StoreTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel)
{
	try
	{
		auto data = blob.GetData();
		auto size = blob.GetSize();
		if(!data)
			throw std::runtime_error("No texture data to store");
		else if(FileTypeIdentification::IsDds(data, size))
			StoreDdsTexture(target, blob, minLevel, maxLevel);
		else if(FileTypeIdentification::IsKtx(data, size))
			StoreKtxTexture(target, blob, minLevel, maxLevel);
		else if(FileTypeIdentification::IsTga(data, size))
			StoreTgaTexture(target, blob, minLevel, maxLevel);
		else
			throw std::runtime_error("Unknown file type");
	}
	catch(std::runtime_error& e)
	{
		LOG(ERROR) << e.what();
		for(unsigned int i = minLevel; i < std::min(maxLevel-1, +kLodLevels); ++i)
			target.SetState(i, TextureManager::Asset::kFailed);
	}
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::StoreTgaTexture(TextureManager::Asset& target, Blob& blob, unsigned int /*minLevel*/, unsigned int /*maxLevel*/)
{
	TgaFile2 file(blob.GetData(), blob.GetSize());
	const uint8_t* imageData = static_cast<const uint8_t*>(file.GetImageData());
	std::vector<uint8_t> rotatedData;
	if(file.IsUpsideDown())
	{
		rotatedData.resize(file.GetImageSize());
		// TODO: Do in separate task
		const uint8_t* inputData = static_cast<const uint8_t*>(file.GetImageData());
		// Read image bottom-up
		size_t lineSize = file.GetWidth() * file.GetBytesPerPixel();
		unsigned int height = file.GetHeight();
		for(size_t line = 0; line < height; ++line)
			memcpy(rotatedData.data() + (height - line - 1) * lineSize, inputData + line * lineSize, lineSize);
		imageData = rotatedData.data();
	}
	target.GetAsset()->Store(file.GetWidth(), file.GetHeight(), imageData, file.GetFormat());
	target.GetAsset()->GenerateMipmaps();
	for(unsigned int i = 0; i < +kLodLevels; ++i)
		target.SetState(i, TextureManager::Asset::kLoaded);
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::StoreKtxTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel)
{
	KtxFile ktxFile(blob.GetData(), blob.GetSize());
	unsigned int width = ktxFile.GetPixelWidth();
	unsigned int height = ktxFile.GetPixelHeight();
	auto glType = ktxFile.GetGlType();
	auto glFormat = ktxFile.GetGlFormat();
	auto glInternalFormat = ktxFile.GetGlInternalFormat();
	PixelFormat format = PixelFormatConversion::ToPixelFormat(glType, glFormat, glInternalFormat);
	unsigned int levels = std::min(maxLevel + 1, ktxFile.GetNumberOfMipmapLevels());
	for(unsigned int i = minLevel; i < levels; ++i)
	{
		auto mipLevel = ktxFile.GetImageData(i);
		target.GetAsset()->Store(width, height, mipLevel.first, format, i, mipLevel.second);
		if(i < +kLodLevels)
			target.SetState(i, TextureManager::Asset::kLoaded);
		width /= 2;
		height /= 2;
	}
}

template<class TRenderManager>
void TextureLoader<TRenderManager>::StoreDdsTexture(TextureManager::Asset& target, Blob& blob, unsigned int minLevel, unsigned int maxLevel)
{
	util::DdsFile file(blob.GetData(), blob.GetSize());
	unsigned int levelsToLoad = std::min(maxLevel + 1, file.GetNumMipmapLevels());
	for(unsigned int i = minLevel; i < levelsToLoad; ++i)
	{
		unsigned int width = 0, height = 0;
		size_t size = 0;
		const void* pointer = file.GetSingleImage(0, i, width, height, size);
		target.GetAsset()->Store(width, height, pointer, file.GetFormat(), i, size);
		if(i < +kLodLevels)
			target.SetState(i, TextureManager::Asset::kLoaded);
	}
}

}
}

#endif // MOLECULAR_TEXTUREMANAGER_H
