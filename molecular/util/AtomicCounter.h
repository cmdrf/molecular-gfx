/*  AtomicCounter.h

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

#ifndef MOLECULAR_ATOMICCOUNTER_H
#define MOLECULAR_ATOMICCOUNTER_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <windows.h>
#undef SendMessage

namespace molecular
{
namespace util
{

/// Counter with atomic increment/decrement methods
/** This implementation uses the Windows API. */
class WinAtomicCounter
{
public:
	inline void Increment()
	{
		InterlockedIncrement(&mCounter);
	}

	inline void Decrement()
	{
		InterlockedDecrement(&mCounter);
	}

	inline bool DecrementCheckZero()
	{
		return InterlockedDecrement(&mCounter) <= 0;
	}

	inline bool CheckZero() { return (mCounter <= 0); }

private:
	__declspec(align(4)) LONG mCounter = 0;
};

typedef WinAtomicCounter AtomicCounter;
} // namespace
}
#else

namespace molecular
{
namespace util
{

/// Counter with atomic increment/decrement methods
/** This implementation uses GCC compiler intrinsics (also supported on clang).
	*/
class GccAtomicCounter
{
public:
	/// Increments Counter by one.
	inline void Increment() {__sync_add_and_fetch(&mCounter, 1);}
	
	/// Decrements Counter by one.
	inline void Decrement() {__sync_sub_and_fetch(&mCounter, 1);}

	/// Decrements counter and checks for zero
	/** @return true if counter reaches zero. */
	inline bool DecrementCheckZero() {return (__sync_sub_and_fetch(&mCounter, 1) <= 0);}

	inline bool CheckZero() {return (mCounter <= 0);}
private:
	long mCounter = 0;
};

typedef GccAtomicCounter AtomicCounter;

}
} // namespace

#endif // _WIN32

#endif
