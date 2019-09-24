/*	GlFunctions.h

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

#ifndef MOLECULAR_GLFUNCTIONS_H
#define MOLECULAR_GLFUNCTIONS_H

#include "GlFunctionsProcAddresses.h"
#include "GlConstants.h"
#include <stdexcept>

#if _WIN32

#include "GlFunctionsGl10Native.h"
#include "GlFunctionsGles2ProcAddresses.h"

namespace molecular
{

struct GlFunctionsInitializerWgl
{
	template<class T>
	static void Init(T& procAddress, const char* name, bool optional = false, const char* secondaryName = nullptr)
	{
		procAddress = T(wglGetProcAddress(name));
		if (!procAddress)
		{
			if (secondaryName)
				procAddress = T(wglGetProcAddress(secondaryName));
			if (!procAddress && !optional)
				throw std::runtime_error(std::string(name) + " not available");
		}
	}
};

class GlFunctions :
	public GlFunctionsGl10Native,
	public GlFunctionsGles2ProcAddresses<GlFunctionsInitializerWgl>,
	public GlFunctionsProcAddresses<GlFunctionsInitializerWgl>,
	public GlConstants
{
public:
	void Init()
	{
		GlFunctionsProcAddresses<GlFunctionsInitializerWgl>::Init();
		GlFunctionsGles2ProcAddresses<GlFunctionsInitializerWgl>::Init();
	}

	void ReadBuffer(GLenum mode) { glReadBuffer(mode); }
};

}

#elif __APPLE__

#include "GlFunctionsGles2Native.h"
#include "GlFunctionsNative.h"

namespace molecular
{

class GlFunctions :
	public GlFunctionsNative,
	public GlFunctionsGles2Native,
	public GlConstants
{};

#elif defined(OPENGL_ES3)

#include "Egl.h"
#include "GlFunctionsGles2Native.h"

/// Acquires procedure pointers through eglGetProcAddress
/** Relies on the EGL_KHR_get_all_proc_addresses EGL extension. */
struct GlFunctionsInitializerEgl
{
	template<class T>
	static void Init(T& procAddress, const char* name, bool optional = false, const char* secondaryName = nullptr)
	{
		procAddress = T(Egl::GetProcAddress(name));
		if(!procAddress)
		{
			if(secondaryName)
				procAddress = T(Egl::GetProcAddress(secondaryName));
			if(!procAddress && !optional)
				throw std::runtime_error(std::string(name) + " not available");
		}
	}
};

class GlFunctions :
	public GlFunctionsProcAddresses<GlFunctionsInitializerEgl>,
	public GlFunctionsGles2Native,
	public GlConstants
{};

}

#else // Linux

#include "Glx.h"
#include "GlFunctionsGles2Native.h"

namespace molecular
{

struct GlFunctionsInitializerGlx
{
	template<class T>
	static void Init(T& procAddress, const char* name, bool optional = false, const char* secondaryName = nullptr)
	{
		procAddress = T(Glx::GetProcAddress(name));
		if (!procAddress)
		{
			if (secondaryName)
				procAddress = T(Glx::GetProcAddress(secondaryName));
			if (!procAddress && !optional)
				throw std::runtime_error(std::string(name) + " not available");
		}
	}
};

class GlFunctions :
	public GlFunctionsProcAddresses<GlFunctionsInitializerGlx>,
	public GlFunctionsGles2Native,
	public GlConstants
{};

}

#endif // defined(OPENGL_ES3) && !defined(__APPLE__)

#endif // GLFUNCTIONSGLES3_H
