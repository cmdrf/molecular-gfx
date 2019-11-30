/*	TestIteratorAdapters.cpp

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

#include <catch.hpp>
#include <vector>
#include <molecular/util/IteratorAdapters.h>
#include <iostream>

using namespace molecular::util;

TEST_CASE("TestSkippingIterator")
{
	std::vector<int> numbers = {45, 231, 34, 389, 321, 498, 4};

	auto isOdd = [](int i){return i & 1;};

	auto evenBegin = MakeSkippingIterator(numbers.begin(), numbers.end(), isOdd);
	auto evenEnd = MakeSkippingIterator(numbers.end(), numbers.end(), isOdd);

	std::vector<int> outNumbers;
	for(auto it = evenBegin; it != evenEnd; ++it)
	{
		outNumbers.push_back(*it);
	}

	CHECK(3 == outNumbers.size());
	CHECK(34 == outNumbers.at(0));
	CHECK(498 == outNumbers.at(1));
	CHECK(4 == outNumbers.at(2));
}
