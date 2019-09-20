/*	TextureSplattingData.h
	Copyright 2012-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TEXTURESPLATTINGDATA_H
#define TEXTURESPLATTINGDATA_H

#include <molecular/programgenerator/ProgramGenerator.h>

namespace molecular
{
class TextureSplattingData
{
public:
	static void FeedToGenerator(ProgramGenerator& generator);
};

}

#endif // TEXTURESPLATTINGDATA_H
