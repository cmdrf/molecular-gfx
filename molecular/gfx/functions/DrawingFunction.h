/*	DrawingFunction.h
	Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DRAWINGFUNCTION_H
#define DRAWINGFUNCTION_H

#include <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace Gfx
{

/// Base class for functions drawing stuff
class DrawingFunction : public RenderFunction
{
public:

	template<class TRenderManager>
	DrawingFunction(TRenderManager& manager) :
		RenderFunction(manager),
		mProgramProvider(manager.GetProgramProvider()),
		mLastBoundsChange(manager.GetFramecounter())
	{}

	bool BoundsChangedSince(int framecounter) const override {return mLastBoundsChange > framecounter;}

protected:
	/** The return value is usually ignored. */
	RenderCmdSink::Program* PrepareProgram()
	{
		auto getArraySize = [&](Hash hash)
		{
			const Variable* var = mScoping[hash];
			assert(var);
			return var->GetArraySize();
		};

		auto program = mProgramProvider.GetProgram(mScoping.KeyBegin(), mScoping.KeyEnd(), getArraySize);
		assert(program);
		mRenderer.UseProgram(program);

		for(auto input: program->GetInputs())
		{
			const Variable* var = mScoping[input];
			if(var)
				var->Apply(program, input);
		}
		return program;
	}

protected:
	ProgramProvider& mProgramProvider;

private:
	int mLastBoundsChange = 0;
};

}
}

#endif // DRAWINGFUNCTION_H

