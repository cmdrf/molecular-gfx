/*	Material.h
	Copyright 2013-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <molecular/util/DynamicScoping.h>
#include "TextureManager.h"
#include "Scoping.h"

namespace molecular
{
namespace Gfx
{

/// Collection of variables
class Material
{
public:
	template<class TRenderManager>
	explicit Material(TRenderManager& renderManager) : mTextureManager(renderManager.GetTextureManager()), mScoping(renderManager.GetScoping()) {}

	Material(TextureManager& textureManager, Gfx::Scoping& scoping) : mTextureManager(textureManager), mScoping(scoping) {}
	~Material();

	template<class T>
	void SetUniform(Hash hash, const T& variable)
	{
		assert(!mBound);
		int index = FindEntry(hash);
		using Binding = Gfx::Scoping::ManualBinding<Uniform<T>>;
		if(index == -1)
		{
			Binding* binding = new Binding(hash, mScoping);
			***binding = variable;
			mUniforms.emplace_back(hash, binding);
		}
		else
		{
			***static_cast<Binding*>(mUniforms[index].binding.get()) = variable;
		}
	}

	void SetTexture(Hash hash, Hash textureFileName);

	void Bind(int lodLevel = 0);
	void Unbind();

private:
	struct Entry
	{
		Entry(){}
		Entry(Hash n, Gfx::Scoping::SkeletalManualBinding* b, TextureManager::Asset* t = nullptr) :
			name(n), binding(b), texture(t){}

		Hash name = 0;
		std::unique_ptr<Gfx::Scoping::SkeletalManualBinding> binding;
		TextureManager::Asset* texture = nullptr;
	};

	int FindEntry(Hash name);

	std::vector<Entry> mUniforms;
	TextureManager& mTextureManager;
	Gfx::Scoping& mScoping;
	bool mBound = false;
};

class MaterialBinding
{
public:
	explicit MaterialBinding(Material& material, int lodLevel = 0) : mMaterial(material) {material.Bind(lodLevel);}
	~MaterialBinding() {mMaterial.Unbind();}

private:
	Material& mMaterial;
};

}
}

#endif // MATERIAL_H
