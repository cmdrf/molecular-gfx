/*	GlFunctions.h
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include "GlFunctionsProcAddresses.h"
#include "GlConstants.h"
#include <stdexcept>

#if _WIN32

#include "GlFunctionsGl10Native.h"
#include "GlFunctionsGles2ProcAddresses.h"

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

#elif __APPLE__

#include "GlFunctionsGles2Native.h"
#include "GlFunctionsNative.h"

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

#else // Linux

#include "Glx.h"
#include "GlFunctionsGles2Native.h"

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

#endif // defined(OPENGL_ES3) && !defined(__APPLE__)

#endif // GLFUNCTIONSGLES3_H
