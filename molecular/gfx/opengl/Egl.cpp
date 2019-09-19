/*	Egl.cpp
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "OpenGlPrerequisites.h"
#include "Egl.h"
#include <cassert>

#if defined(OPENGL_ES3) && !defined(__APPLE__)
#include <EGL/egl.h>

namespace Egl
{
ProcAddress GetProcAddress(const char* procname) {return eglGetProcAddress(procname);}
}

#else
Egl::ProcAddress Egl::GetProcAddress(const char*) {assert(false); return nullptr;}
#endif
