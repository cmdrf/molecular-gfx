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

}

#endif // MOLECULAR_ITERATORADAPTERS_H
