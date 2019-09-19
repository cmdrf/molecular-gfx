/*	DefaultProgramData.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DEFAULTPROGRAMDATA_H
#define DEFAULTPROGRAMDATA_H

#include <molecular/programgenerator/ProgramGenerator.h>

/// Contains default data for ProgramGenerator
class DefaultProgramData
{
public:
	static void FeedToGenerator(molecular::ProgramGenerator& generator);
};

#endif // DEFAULTPROGRAMDATA_H
