/*	Scope.h

MIT License

Copyright (c) 2020 Fabian Herb

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

#ifndef MOLECULAR_UTIL_SCOPE_H
#define MOLECULAR_UTIL_SCOPE_H

#include <molecular/util/Hash.h>

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

namespace molecular
{
namespace util
{

template<class BaseType>
class Scope
{
public:
	Scope(const Scope* parent = nullptr) : mParent(parent) {}

	template<class SubType>
	SubType& Bind(Hash key);

	template<class SubType>
	const SubType& Get(Hash key) const;

	template<class SubType>
	void Set(Hash key, SubType&& value)
	{
		auto it = std::find(mKeys.begin(), mKeys.end(), key);
		if(it != mKeys.end())
			*mValues[std::distance(mKeys.begin(), it)] = value;
		else
		{
			mKeys.push_back(key);
			mValues.emplace_back(new SubType(value));
		}
	}

	template<class SubType>
	void Set(Hash key, const SubType& value)
	{
		auto it = std::find(mKeys.begin(), mKeys.end(), key);
		if(it != mKeys.end())
			*mValues[std::distance(mKeys.begin(), it)] = value;
		else
		{
			mKeys.push_back(key);
			mValues.emplace_back(new SubType(value));
		}
	}

	void Unset(Hash key)
	{
		auto it = std::find(mKeys.begin(), mKeys.end(), key);
		if(it != mKeys.end())
			mValues.at(std::distance(mKeys.begin(), it)).reset();
		else
		{
			mValues.emplace_back();
			mKeys.push_back(key);
		}
	}

	bool Has(Hash key) const
	{
		auto it = std::find(mKeys.begin(), mKeys.end(), key);
		if(it != mKeys.end())
			return true;
		if(mParent)
			return mParent->Has(key);
		else
			return false;
	}

	std::map<Hash, BaseType*> ToMap() const
	{
		std::map<Hash, BaseType*> out;
		if(mParent)
			out = mParent->ToMap();
		for(size_t i = 0; i < mKeys.size(); ++i)
		{
			if(mValues[i])
				out[mKeys[i]] = mValues[i].get();
			else
				out.erase(mKeys[i]); // Erase variable from parent scope if it was Unset() in this scope
		}
		return out;
	}

private:
	std::vector<Hash> mKeys;
	std::vector<std::unique_ptr<BaseType>> mValues;
	const Scope* mParent = nullptr;
};

template<class BaseType>
template<class SubType>
SubType& Scope<BaseType>::Bind(Hash key)
{
	assert(std::find(mKeys.begin(), mKeys.end(), key) == mKeys.end() && "Variable already bound in this scope");

	mKeys.push_back(key);
	if(mParent && mParent->Has(key))
		mValues.emplace_back(new SubType(mParent->Get<SubType>(key)));
	else
		mValues.emplace_back(new SubType());
	return static_cast<SubType&>(*mValues.back());
}

template<class BaseType>
template<class SubType>
const SubType& Scope<BaseType>::Get(Hash key) const
{
	auto it = std::find(mKeys.begin(), mKeys.end(), key);
	if(it != mKeys.end())
		return static_cast<SubType&>(*mValues.at(std::distance(mKeys.begin(), it)));
	if(mParent)
		return mParent->Get<SubType>(key);
	else
		throw std::runtime_error("Variable not declared");
}

}
}

#endif // MOLECULAR_UTIL_SCOPE_H
