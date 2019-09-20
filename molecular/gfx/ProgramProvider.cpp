/*	ProgramProvider.cpp
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "ProgramProvider.h"
#include "DefaultProgramData.h"

namespace molecular
{
namespace gfx
{

ProgramProvider::ProgramProvider(RenderCmdSink& renderer, ProgramGenerator& generator) :
	mRenderer(renderer),
	mGenerator(generator)
{
}

ProgramProvider::~ProgramProvider()
{
	for(auto& it: mGeneratedPrograms)
	{
		mRenderer.DestroyProgram(it.second);
	}
}

}
}
