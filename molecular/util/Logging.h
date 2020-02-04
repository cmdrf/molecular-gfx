/*	Logging.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#ifndef MOLECULAR_UTIL_LOGGING_H
#define MOLECULAR_UTIL_LOGGING_H

#include <iostream>

namespace molecular
{
namespace util
{

/// Automatic newlines for log messages
/** Based on an answer by user7860670 to https://stackoverflow.com/questions/51802549 */
struct Log
{
	Log(const char* severity)
	{
		std::cerr << severity << ": ";
	}

	~Log() { std::cerr << std::endl; }
};



template<typename T> Log &&
operator <<(Log && wrap, T const & whatever)
{
	std::cerr << whatever;
	return std::move(wrap);
}

}
}

#define LOG(x) molecular::util::Log(#x)

#endif // MOLECULAR_UTIL_LOGGING_H
