/*	SetUniforms.cpp
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "SetUniforms.h"

namespace molecular
{
void Gfx::SetUniforms::Execute()
{
	if(mCallee)
	{
		MaterialBinding binding(mUniforms);
		mCallee->Execute();
	}
}
}
