/*	DrawingFunction.h

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

#ifndef MOLECLUAR_DRAWINGFUNCTION_H
#define MOLECLUAR_DRAWINGFUNCTION_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/IteratorAdapters.h>

namespace molecular
{
namespace gfx
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
	RenderCmdSink::Program* PrepareProgram(const Scope& scope)
	{
		auto getArraySize = [&](Hash hash)
		{
			assert(scope.Has(hash));
			return scope.Get<Variable>(hash).GetArraySize();
		};

		auto scopeMap = scope.ToMap();
		auto program = mProgramProvider.GetProgram(util::MakePairFirstIterator(scopeMap.begin()), util::MakePairFirstIterator(scopeMap.end()), getArraySize);
		assert(program);
		mRenderer.UseProgram(program);

		for(auto input: program->GetInputs())
		{
			assert(scope.Has(input));
			scope.Get<Variable>(input).Apply(program, input);
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

#endif // MOLECLUAR_DRAWINGFUNCTION_H

