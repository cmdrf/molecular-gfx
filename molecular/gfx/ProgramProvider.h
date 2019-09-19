/*	ProgramProvider.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PROGRAMPROVIDER_H
#define PROGRAMPROVIDER_H

#include <molecular/util/Hash.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include "RenderCmdSink.h"
#include <molecular/util/NonCopyable.h>

namespace molecular
{
namespace Gfx
{

/// Caches programs from the ProgramGenerator
/** @todo Feeding different available inputs (of which not all are used) to the
		ProgramGenerator may result in equal programs that are stored multiple
		times by the ProgramProvider. */
class ProgramProvider : NonCopyable
{
public:
	ProgramProvider(RenderCmdSink& renderer, molecular::ProgramGenerator& generator);
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
	molecular::ProgramGenerator& mGenerator;

	std::unordered_map<Hash, RenderCmdSink::Program*> mGeneratedPrograms;
	std::unordered_map<molecular::ProgramGenerator::Variable,int> mArraySizes;
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
		molecular::ProgramGenerator::ProgramText text = mGenerator.GenerateProgram(varsBegin, varsEnd, mArraySizes);
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
