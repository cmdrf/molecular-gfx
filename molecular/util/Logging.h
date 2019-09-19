/*	Logging.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef LOGGING_H
#define LOGGING_H

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#endif

#define ELPP_THREAD_SAFE
#if !defined(__ANDROID__) && !defined(_WIN32)
#define ELPP_STACKTRACE_ON_CRASH
#endif
#define ELPP_CUSTOM_COUT std::cerr

#include <easylogging++.h>

// Undefine windows garbage:
#undef SendMessage
#undef GetObject
#undef NO_ERROR
#undef min

namespace Logging
{
void Initialize(int argc, char* argv[], const std::string& logfile = "");
}

#endif // LOGGING_H
