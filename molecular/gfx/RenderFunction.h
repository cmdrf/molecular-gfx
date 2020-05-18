/*	RenderFunction.h

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

#ifndef MOLECULAR_GFX_RENDERFUNCTION_H
#define MOLECULAR_GFX_RENDERFUNCTION_H

#include "ProgramProvider.h"
#include "Uniform.h"
#include "Scope.h"

#include <molecular/util/AxisAlignedBox.h>

namespace molecular
{
namespace gfx
{

/// Function object that issues draw calls and calls other RenderFunctions
class RenderFunction
{
public:
	template<class TRenderManager>
	explicit RenderFunction(TRenderManager& manager) :
		mRenderer(manager.GetRenderCmdSink()),
		mTextureManager(manager.GetTextureManager())
	{}

//	RenderFunction(RenderCmdSink& renderer) : mRenderer(renderer)  {}

	virtual ~RenderFunction() {}
	void Execute(const Scope& parentScope);
	virtual util::AxisAlignedBox GetBounds() const = 0;
	virtual bool BoundsChangedSince(int /*framecounter*/) const {return false;}

	void SetVisible(bool visible) {mVisible = visible;}

	/// Set bool property with given name
	/** This system is not finished. */
	virtual void Set(Hash /*variable*/, bool /*value*/) {throw std::runtime_error("This RenderFunction does not have bool parameters");}

	/// Set uniform with given name
	template<class T>
	void SetUniform(Hash name, const T& value)
	{
		mObjectScope.Set(name, Uniform<T>(value));
	}

	/// Add new texture or replace existing texture
	/** @param name Hashed uniform name to be used in the shader program
		@param texture Hashed file path that is passed to FileLoader */
	void SetTexture(Hash name, Hash texture);

	void SetTextureParameter(Hash variable, RenderCmdSink::Texture::Parameter param, RenderCmdSink::Texture::ParamValue value);

protected:
	virtual void HandleExecute(Scope& scope) = 0;

//	/// Call from subclasses when bounds or transforms changed
//	void BoundsChanged() {mBoundsChangedCounter = mRenderManager.GetFramecounter();}

	RenderCmdSink& mRenderer;
	TextureManager& mTextureManager;
//	int mBoundsChangedCounter;
	bool mVisible = true;

private:
	Scope mObjectScope;
};

/// RenderFunction that calls a single other RenderFunction only
class SingleCalleeRenderFunction : public RenderFunction
{
public:
	template<class TRenderManager>
	SingleCalleeRenderFunction(TRenderManager& manager) : RenderFunction(manager) {}

//	SingleCalleeRenderFunction(RenderCmdSink& renderer) : RenderFunction(renderer), mCallee(nullptr) {}

	molecular::util::AxisAlignedBox GetBounds() const override {return mCallee->GetBounds();}
	bool BoundsChangedSince(int framecounter) const override {return mCallee->BoundsChangedSince(framecounter);}

	void SetCallee(RenderFunction* callee) {mCallee = callee;}

protected:
	RenderFunction* mCallee = nullptr;

};

/// RenderFunction that calls multiple other RenderFunctions
/** Why not make all RenderFunctions MultipleCalleeRenderFunctions? Because only those doing
	visibility determination should be used to contain multiple callees, like FlatScene. Why
	not do visibility determination everywhere? Because some functions don't even get a frustum,
	like full-screen effects. */
class MultipleCalleeRenderFunction : public RenderFunction
{
public:
	template<class TRenderManager>
	MultipleCalleeRenderFunction (TRenderManager& manager) : RenderFunction(manager) {}

//	MultipleCalleeRenderFunction (RenderCmdSink& renderer) : RenderFunction(renderer){}

	molecular::util::AxisAlignedBox GetBounds() const override;

	/// @todo implement
	bool BoundsChangedSince(int /*framecounter*/) const override {return true;}

	void AppendCallee(RenderFunction* callee) {assert(callee); mCallees.insert(callee);}
	void RemoveCallee(RenderFunction* callee) {assert(callee); mCallees.erase(callee);}
	void ClearCallees() {mCallees.clear();}

protected:
	std::unordered_set<RenderFunction*> mCallees;

};

}
}

#endif // RENDERFUNCTION_H
