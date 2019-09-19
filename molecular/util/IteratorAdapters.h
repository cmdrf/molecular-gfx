/*	IteratorAdapters.h
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef KEYITERATOR_H
#define KEYITERATOR_H

#include <iterator>

/// Iterator adapter that dereferences to the first value of a pair iterator
template<class PairIterator>
class PairFirstIterator : public std::iterator<std::input_iterator_tag, typename PairIterator::value_type::first_type>
{
public:
	PairFirstIterator() {}
	PairFirstIterator(PairIterator it) : mIt(it) {}

	typedef typename PairIterator::value_type::first_type value_type;

	PairFirstIterator& operator++() {++mIt; return *this;}
	const value_type& operator*() const {return mIt->first;}
//	const PairFirstIterator::value_type::first_type& operator->() const {return mIt->first;}
	bool operator==(const PairFirstIterator& it) const {return it.mIt == mIt;}
	bool operator!=(const PairFirstIterator& it) const {return it.mIt != mIt;}

private:
	PairIterator mIt;
};

/// Iterator adapter that dereferences to the second value of a pair iterator
template<class PairIterator>
class PairSecondIterator : public std::iterator<std::input_iterator_tag, typename PairIterator::value_type::second_type>
{
public:
	PairSecondIterator() {}
	PairSecondIterator(PairIterator it) : mIt(it) {}

	typedef typename PairIterator::value_type::second_type value_type;

	PairSecondIterator& operator++() {++mIt; return *this;}
	const value_type& operator*() const {return mIt->second;}
//	const PairFirstIterator::value_type::first_type& operator->() const {return mIt->first;}
	bool operator==(const PairSecondIterator& it) const {return it.mIt == mIt;}
	bool operator!=(const PairSecondIterator& it) const {return it.mIt != mIt;}

private:
	PairIterator mIt;
};

/// Iterator that skips certain positions
/** Skips all elements for which the predicate returns true */
template<class Iterator, class Predicate>
class SkippingIterator : public std::iterator<std::input_iterator_tag, typename Iterator::value_type>
{
public:
	typedef typename Iterator::value_type value_type;

	SkippingIterator() {}
	SkippingIterator(Iterator it, Iterator end, Predicate pred) :
		mIt(it), mEnd(end), mPredicate(pred)
	{
		while (mIt != mEnd && mPredicate(*mIt))
			++mIt;
	}

	SkippingIterator& operator++()
	{
		do
		{
			++mIt;
		} while (mIt != mEnd && mPredicate(*mIt));
		return *this;
	}

	bool operator==(const SkippingIterator& it) const {return it.mIt == mIt;}
	bool operator!=(const SkippingIterator& it) const {return it.mIt != mIt;}
	value_type& operator*() {return *mIt;}
	const value_type& operator*() const {return *mIt;}
	value_type* operator->() {return &*mIt;}
	const value_type* operator->() const {return &*mIt;}

private:
	Iterator mIt, mEnd;
	Predicate mPredicate;
};

/// Simple wrapper around SkippingIterator::SkippingIterator for using template parameter deduction
template<class Iterator, class Predicate>
SkippingIterator<Iterator, Predicate> MakeSkippingIterator(Iterator it, Iterator end, Predicate pred)
{
	return SkippingIterator<Iterator, Predicate>(it, end, pred);
}

#endif // KEYITERATOR_H
