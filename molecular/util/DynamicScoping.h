/*	DynamicScoping.h
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DYNAMICSCOPING_H
#define DYNAMICSCOPING_H

#include <map>
#include <cassert>
#include "IteratorAdapters.h"

/// Implementation of dynamic scoping
/** As opposed to static (lexical) scoping used in C/C++. Shallow binding. */
template<class _Key, class _ValueSuperType = void>
class DynamicScoping
{
public:
	typedef _Key Key;
	typedef _ValueSuperType ValueSuperType;

private:
	/** This cannot be unordered_map because pointers to individual elements are kept in bindings. */
	typedef typename std::map<Key, ValueSuperType*> Map;

	/// Functor for use with the SkippingIterator
	class IsSecondNull // : public std::unary_function<bool, ?>
	{
	public:
		template<class Pair>
		bool operator() (const Pair& it) const {return (it.second == nullptr);}
	};

public:
	const ValueSuperType* operator[](const Key& key) {return mMap[key];}

	/// Binding with external data storage
	/** Use with care. Better use Binding instead. */
	class SkeletalManualBinding
	{
	public:
		SkeletalManualBinding(Key key, DynamicScoping& scoping, ValueSuperType* value) :
			mValue(value),
			mPointerLocation(&scoping.mMap[key])
		{

		}

		void Bind()
		{
			// Save current pointer:
			mOriginalPointer = *mPointerLocation;
			// Install own:
			*mPointerLocation = mValue;
		}

		void Unbind()
		{
			assert(*mPointerLocation == mValue);
			// Restore old pointer:
			*mPointerLocation = mOriginalPointer;
		}

	private:
		ValueSuperType* mValue;
		ValueSuperType** mPointerLocation;

	protected:
		ValueSuperType* mOriginalPointer = nullptr;
	};

	/** Use with care. Better use Binding instead. */
	template<class ValueSubType>
	class ManualBinding : public SkeletalManualBinding
	{
	public:
		ManualBinding(Key key, DynamicScoping& scoping) :
			SkeletalManualBinding(key, scoping, &mCurrentValue)
		{

		}

		void operator=(ValueSubType value) {mCurrentValue = value;}
		ValueSubType& operator* () {return mCurrentValue;}
		ValueSubType* operator->() {return &mCurrentValue;}

	protected:
		ValueSubType mCurrentValue;
	};

	/// Automatically binds a variable to the current scope
	/** Use this only on the stack. Access the contained data by dereferencing
		the Binding. */
	template<class ValueSubType>
	class Binding : protected ManualBinding<ValueSubType>
	{
	public:
		Binding(Key key, DynamicScoping& scoping) :
			ManualBinding<ValueSubType>(key, scoping)
		{
			this->Bind();
			if(this->mOriginalPointer)
			{
				// Copy value of previous instance:
				const ValueSubType* original = static_cast<const ValueSubType*>(this->mOriginalPointer);
				this->mCurrentValue = *original;
			}
		}

		~Binding()
		{
//			this->template Unbind();
			this->Unbind();
		}

		void operator=(ValueSubType value) {this->mCurrentValue = value;}
		ValueSubType& operator* () {return this->mCurrentValue;}
		ValueSubType* operator->() {return &this->mCurrentValue;}
	};

	/// Binding without its own copy of the data
	class SkeletalBinding : protected SkeletalManualBinding
	{
	public:
		SkeletalBinding(Key key, DynamicScoping& scoping, ValueSuperType* value) :
			SkeletalManualBinding(key, scoping, value)
		{
			this->Bind();
		}

		~SkeletalBinding()
		{
			this->Unbind();
		}

	};

	/// Skips entries that are not set
	typedef SkippingIterator<typename Map::const_iterator, IsSecondNull> Iterator;

	typedef PairFirstIterator<Iterator> KeyIterator;

	Iterator Begin() const {return Iterator(mMap.begin(), mMap.end(), IsSecondNull());}
	Iterator End() const {return Iterator(mMap.end(), mMap.end(), IsSecondNull());}

	KeyIterator KeyBegin() {return KeyIterator(Begin());}
	KeyIterator KeyEnd() {return KeyIterator(End());}

private:
	Map mMap;
};

#endif // DYNAMICSCOPING_H
