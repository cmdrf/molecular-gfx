/*  AtomicCounter.h
	Copyright 2010-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef ATOMICCOUNTER_H
#define ATOMICCOUNTER_H

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <windows.h>
	#undef SendMessage

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
#else

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

#endif // _WIN32

#endif // ATOMICCOUNTER_H
