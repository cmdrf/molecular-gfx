/*	Material.h

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

#ifndef MOLECULAR_MATERIAL_H
#define MOLECULAR_MATERIAL_H

#include <molecular/util/DynamicScoping.h>
#include "TextureManager.h"
#include "Scoping.h"

namespace molecular
{
namespace gfx
{

/// Collection of variables
class Material
{
public:
	template<class TRenderManager>
	explicit Material(TRenderManager& renderManager) : mTextureManager(renderManager.GetTextureManager()), mScoping(renderManager.GetScoping()) {}

	Material(TextureManager& textureManager, gfx::Scoping& scoping) : mTextureManager(textureManager), mScoping(scoping) {}
	~Material();

	template<class T>
	void SetUniform(Hash hash, const T& variable)
	{
		assert(!mBound);
		int index = FindEntry(hash);
		using Binding = gfx::Scoping::ManualBinding<Uniform<T>>;
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
		Entry(Hash n, gfx::Scoping::SkeletalManualBinding* b, TextureManager::Asset* t = nullptr) :
			name(n), binding(b), texture(t){}

		Hash name = 0;
		std::unique_ptr<gfx::Scoping::SkeletalManualBinding> binding;
		TextureManager::Asset* texture = nullptr;
	};

	int FindEntry(Hash name);

	std::vector<Entry> mUniforms;
	TextureManager& mTextureManager;
	gfx::Scoping& mScoping;
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
