/*	RequestOutput.cpp
	Copyright 2014-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "RequestOutput.h"

namespace Gfx
{

void RequestOutput::Execute()
{
	Binding<Output> fragmentColor(mOutput, this);
	Binding<Output> glPosition("gl_Position"_H, this);

	if(mCallee)
		mCallee->Execute();
}

}
