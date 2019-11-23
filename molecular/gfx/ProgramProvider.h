/*	ProgramProvider.h

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

#ifndef MOLECULAR_PROGRAMPROVIDER_H
#define MOLECULAR_PROGRAMPROVIDER_H

#include <molecular/util/Hash.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include "RenderCmdSink.h"
#include <molecular/util/NonCopyable.h>

namespace molecular
{
namespace gfx
{

/// Caches programs from the ProgramGenerator
/** @todo Feeding different available inputs (of which not all are used) to the
		ProgramGenerator may result in equal programs that are stored multiple
		times by the ProgramProvider. */
class ProgramProvider : NonCopyable
{
public:
	ProgramProvider(RenderCmdSink& renderer, programgenerator::ProgramGenerator& generator);
	~ProgramProvider();

	/// Get program with the given inputs and outputs
	/** If no program with these variables has been created yet, generates one
		and cashes it.
		@param arraySizeFunc Function that returns an array size for a given variable hash. */
	template<class Iterator, typename ArraySizeFunc>
	RenderCmdSink::Program* GetProgram(Iterator varsBegin, Iterator varsEnd, ArraySizeFunc&& arraySizeFunc);

private:
	template<class Iterator>
	static Hash CalculateHash(Iterator begin, Iterator end);

	RenderCmdSink& mRenderer;
	programgenerator::ProgramGenerator& mGenerator;

	std::unordered_map<Hash, RenderCmdSink::Program*> mGeneratedPrograms;
	std::unordered_map<programgenerator::ProgramGenerator::Variable,int> mArraySizes;
};

template<class Iterator, typename ArraySizeFunc>
RenderCmdSink::Program* ProgramProvider::GetProgram(Iterator varsBegin, Iterator varsEnd, ArraySizeFunc&& arraySizeFunc)
{
	// Calculate hash of all variables:
	Hash hash = CalculateHash(varsBegin, varsEnd);

	// Search for cached program:
	auto it = mGeneratedPrograms.find(hash);
	if(it != mGeneratedPrograms.end())
		return it->second; // Cached program found, return it
	else
	{
		// Program not cached, generate it
		for(auto it = varsBegin; it != varsEnd; ++it)
		{
			if(mArraySizes.count(*it) == 0)
				mArraySizes[*it] = arraySizeFunc(*it);
		}
		programgenerator::ProgramGenerator::ProgramText text = mGenerator.GenerateProgram(varsBegin, varsEnd, mArraySizes);
		RenderCmdSink::Program* program = mRenderer.CreateProgram();
		program->Store(text.vertexShader, text.fragmentShader);
		mGeneratedPrograms[hash] = program;
		return program;
	}
}

template<class Iterator>
Hash ProgramProvider::CalculateHash(Iterator begin, Iterator end)
{
	Iterator it;
	Hash hash = 0;
	for(it = begin; it != end; ++it)
	{
		if(it == begin)
			hash = *it;
		else
			hash = molecular::HashUtils::Combine(hash, *it);
	}

	return hash;
}

}
}

#endif // PROGRAMPROVIDER_H
