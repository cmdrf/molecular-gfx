/*	MaterialManager.h
	Copyright 2013-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <molecular/util/PointerContainer.h>
#include <molecular/util/IniFile.h>
#include <molecular/util/MtlFile.h>
#include <molecular/util/Hash.h>
#include "TextureManager.h"
#include "Scoping.h"

namespace Gfx
{

// Includes not possible here due to circular dependency
class Material;

/// Loads materials from ini files and provides them on request
class MaterialManager
{
public:
	template<class TRenderManager>
	MaterialManager(TRenderManager& renderManager)  : mTextureManager(renderManager.GetTextureManager()), mScoping(renderManager.GetScoping()) {}

	MaterialManager(TextureManager& textureManager, Gfx::Scoping& scoping) : mTextureManager(textureManager), mScoping(scoping) {}
	~MaterialManager();

	template<class TStorage>
	void ReadIniFile(TStorage& file);

	template<class TStorage>
	void ReadMtlFile(TStorage& file);

	void ReadIniFile(const Blob& blob);

	void ReadMtlFile(const Blob& blob);

	void ReadFile(IniFile& file);

	void ReadFile(MtlFile& file);

	Material* GetMaterial(Hash hash) const;
	Material* GetMaterial(const std::string& material) const;

private:
	void AddVariable(Material* material, Hash key, const std::string& value);

	TextureManager& mTextureManager;
	Gfx::Scoping& mScoping;

	std::unordered_map<Hash, std::unique_ptr<Material>> mMaterials;
};


template<class TStorage>
void MaterialManager::ReadIniFile(TStorage& storage)
{
	IniFile file(storage);
	ReadFile(file);
}

template<class TStorage>
void MaterialManager::ReadMtlFile(TStorage& file)
{
	size_t size = file.GetSize();
	std::vector<char> buffer(size);
	file.Read(buffer.data(), size);
	MtlFile mtlFile(buffer.data(), buffer.data() + size);
	ReadFile(mtlFile);
}

}

#endif // MATERIALMANAGER_H
