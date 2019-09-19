/*	Glx.cpp
	Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include <molecular/Config.h>
#include "Glx.h"
#include <cassert>

#if OPENGL_FOUND && !defined(_WIN32) && !defined(__APPLE__)
#include <GL/glx.h>

namespace Glx
{
ProcAddress GetProcAddress(const char* procname) {return glXGetProcAddress(reinterpret_cast<const GLubyte*>(procname));}
}

#else
Glx::ProcAddress Glx::GetProcAddress(const char*) {assert(false); return nullptr;}
#endif
