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
#include "Scoping.h"

#include <molecular/util/AxisAlignedBox.h>

namespace molecular
{
namespace gfx
{

/// Function object that issues draw calls and calls other RenderFunctions
class RenderFunction
{
public:
	/// Convenience shortcut to molecular::gfx::Scoping::Binding
	template<class T>
	class Binding : public Scoping::Binding<T>
	{
	public:
		Binding(Hash key, RenderFunction* self) :
			Scoping::Binding<T>(key, self->mScoping)
		{}

		Binding(const char* key, RenderFunction* self) :
			Scoping::Binding<T>(HashUtils::MakeHash(key), self->mScoping)
		{}
	};

	/// Convenience shortcut to molecular::gfx::Scoping::SkeletalManualBinding
	class SkeletalManualBinding : public Scoping::SkeletalManualBinding
	{
	public:
		SkeletalManualBinding(Hash key, RenderFunction* self, Scoping::ValueSuperType* value) :
			Scoping::SkeletalManualBinding(key, self->mScoping, value)
		{}
	};

	/// Convenience shortcut to molecular::gfx::Scoping::Unbinding
	class Unbinding : public Scoping::Unbinding
	{
	public:
		Unbinding(Hash key, RenderFunction* self) : Scoping::Unbinding(key, self->mScoping) {}
	};

	template<class TRenderManager>
	explicit RenderFunction(TRenderManager& manager) : mScoping(manager.GetScoping()), mRenderer(manager.GetRenderCmdSink()) {}

	RenderFunction(gfx::Scoping& scoping, RenderCmdSink& renderer) : mScoping(scoping), mRenderer(renderer) {}

	virtual ~RenderFunction() {}
	virtual void Execute() = 0;
	virtual util::AxisAlignedBox GetBounds() const = 0;
	virtual bool BoundsChangedSince(int /*framecounter*/) const {return false;}

	/// Set bool property with given name
	/** This system is not finished. */
	virtual void Set(Hash /*variable*/, bool /*value*/) {throw std::runtime_error("This RenderFunction does not have bool parameters");}

protected:
	/// Convenience shortcut to a variable pointer from the dynamic scope
	/** @returns nullptr if variable does not exist. */
	template<class T>
	const T* GetVariable(Hash key)
	{
		return static_cast<const T*>(mScoping[key]);
	}

//	/// Call from subclasses when bounds or transforms changed
//	void BoundsChanged() {mBoundsChangedCounter = mRenderManager.GetFramecounter();}

	gfx::Scoping& mScoping;
	RenderCmdSink& mRenderer;
//	int mBoundsChangedCounter;
};

/// RenderFunction that calls a single other RenderFunction only
class SingleCalleeRenderFunction : public RenderFunction
{
public:
	template<class TRenderManager>
	SingleCalleeRenderFunction(TRenderManager& manager) : RenderFunction(manager) {}

	SingleCalleeRenderFunction(gfx::Scoping& scoping, RenderCmdSink& renderer) : RenderFunction(scoping, renderer), mCallee(nullptr) {}

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

	MultipleCalleeRenderFunction (gfx::Scoping& scoping, RenderCmdSink& renderer) : RenderFunction(scoping, renderer){}

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
