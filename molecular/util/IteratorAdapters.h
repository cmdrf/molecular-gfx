/*	IteratorAdapters.h

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

#ifndef MOLECULAR_ITERATORADAPTERS_H
#define MOLECULAR_ITERATORADAPTERS_H

#include <iterator>

namespace molecular
{
namespace util
{

/// Iterator adapter that dereferences to the first value of a pair iterator
template<class PairIterator>
class PairFirstIterator
{
public:
	PairFirstIterator() {}
	PairFirstIterator(PairIterator it) : mIt(it) {}

	using iterator_category = std::input_iterator_tag;
	using value_type = typename PairIterator::value_type::first_type;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type*;
	using reference = value_type&;

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
class PairSecondIterator
{
public:
	PairSecondIterator() {}
	PairSecondIterator(PairIterator it) : mIt(it) {}

	using iterator_category = std::input_iterator_tag;
	using value_type = typename PairIterator::value_type::second_type;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type*;
	using reference = value_type&;

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
class SkippingIterator
{
public:
	using iterator_category = std::input_iterator_tag;
	using value_type = typename Iterator::value_type;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type*;
	using reference = value_type&;

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

template<class Iterator>
PairFirstIterator<Iterator> MakePairFirstIterator(Iterator it)
{
	return PairFirstIterator<Iterator>(it);
}

}
}

#endif // MOLECULAR_ITERATORADAPTERS_H
