/*	RenderFunction.h
	Copyright 2014-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef RENDERFUNCTION_H
#define RENDERFUNCTION_H

#include "ProgramProvider.h"
#include "Uniform.h"
#include "Scoping.h"

#include <molecular/util/AxisAlignedBox.h>

namespace molecular
{
namespace Gfx
{

/// Function object that issues draw calls and calls other RenderFunctions
class RenderFunction
{
public:
	/// Convenience shortcut to RenderManager::Scoping::Binding
	template<class T>
	class Binding : public Gfx::Scoping::Binding<T>
	{
	public:
		Binding(Hash key, RenderFunction* self) :
			Gfx::Scoping::Binding<T>(key, self->mScoping)
		{}

		Binding(const char* key, RenderFunction* self) :
			Gfx::Scoping::Binding<T>(HashUtils::MakeHash(key), self->mScoping)
		{}
	};

	/// Convenience shortcut to RenderManager::Scoping::SkeletalManualBinding
	class SkeletalManualBinding : public Gfx::Scoping::SkeletalManualBinding
	{
	public:
		SkeletalManualBinding(Hash key, RenderFunction* self, Gfx::Scoping::ValueSuperType* value) :
			Gfx::Scoping::SkeletalManualBinding(key, self->mScoping, value)
		{}
	};

	template<class TRenderManager>
	explicit RenderFunction(TRenderManager& manager) : mScoping(manager.GetScoping()), mRenderer(manager.GetRenderCmdSink()) {}

	RenderFunction(Gfx::Scoping& scoping, RenderCmdSink& renderer) : mScoping(scoping), mRenderer(renderer) {}

	virtual ~RenderFunction() {}
	virtual void Execute() = 0;
	virtual molecular::util::AxisAlignedBox GetBounds() const = 0;
	virtual bool BoundsChangedSince(int /*framecounter*/) const {return false;}

	virtual void Set(Hash variable, bool value) {throw std::runtime_error("This RenderFunction does not have bool parameters");}

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

	Gfx::Scoping& mScoping;
	RenderCmdSink& mRenderer;
//	int mBoundsChangedCounter;
};

/// RenderFunction that calls a single other RenderFunction only
class SingleCalleeRenderFunction : public RenderFunction
{
public:
	template<class TRenderManager>
	SingleCalleeRenderFunction(TRenderManager& manager) : RenderFunction(manager), mCallee(nullptr) {}

	SingleCalleeRenderFunction(Gfx::Scoping& scoping, RenderCmdSink& renderer) : RenderFunction(scoping, renderer), mCallee(nullptr) {}

	molecular::util::AxisAlignedBox GetBounds() const override {return mCallee->GetBounds();}
	bool BoundsChangedSince(int framecounter) const override {return mCallee->BoundsChangedSince(framecounter);}

	void SetCallee(RenderFunction* callee) {mCallee = callee;}

protected:
	RenderFunction* mCallee;

};

}
}

#endif // RENDERFUNCTION_H
