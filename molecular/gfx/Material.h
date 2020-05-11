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

#include "TextureManager.h"
#include "Scope.h"

#include <type_traits>

namespace molecular
{
namespace gfx
{

/// Collection of uniform variables
class Material
{
public:
	template<class TRenderManager>
	explicit Material(TRenderManager& renderManager) : mTextureManager(renderManager.GetTextureManager()) {}

	Material(TextureManager& textureManager) : mTextureManager(textureManager) {}
	~Material();

	template<class T>
	void SetUniform(Hash hash, T&& variable)
	{
		using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

		int index = FindEntry(hash);
		if(index < 0)
		{
			mUniforms.emplace_back(new Uniform<BaseType>(variable));
			mNames.push_back(hash);
		}
		else
			static_cast<Uniform<BaseType>&>(*mUniforms[index]) = variable;
	}

	void SetTexture(Hash hash, Hash textureFileName);

	/// Binds alls material variables to the given Scope
	void Bind(Scope& scope, int lodLevel = 0);

private:
	/** @returns Index in nNames and mUniforms if found, -1 otherwise. */
	int FindEntry(Hash name);

	std::vector<Hash> mNames;
	std::vector<std::unique_ptr<Variable>> mUniforms;
	TextureManager& mTextureManager;
};

}
}

#endif // MATERIAL_H
