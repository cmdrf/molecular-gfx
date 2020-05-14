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

/// Stores variables and optionally has a parent
template<class BaseType>
class Scope
{
public:
	/// Construct Scope with parent
	Scope(const Scope& parent) : mParent(&parent) {}

	/// Construct Scope without parent
	Scope() = default;

	void SetSibling(const Scope& sibling);

	/// Create a variable and get a writable reference
	template<class SubType>
	SubType& Bind(Hash key);

	/// Get value of variable
	template<class SubType>
	const SubType& Get(Hash key) const;

	/// Set variable
	template<class SubType>
	void Set(Hash key, SubType&& value);

	/// Set variable
	template<class SubType>
	void Set(Hash key, const SubType& value);

	/// Unset variable
	/** Unsetting a variable also overrides that variable from parent scopes. */
	void Unset(Hash key);

	/// Determine if the variable is set in this or in parent scopes
	bool Has(Hash key) const;

	/// Get all variables from this scope and its parents
	std::map<Hash, BaseType*> ToMap() const;

private:
	std::vector<Hash> mKeys;
	std::vector<std::unique_ptr<BaseType>> mValues;
	const Scope* mParent = nullptr;
	const Scope* mSibling = nullptr;
};

template<class BaseType>
void Scope<BaseType>::SetSibling(const Scope& sibling)
{
	mSibling = &sibling;
}

template<class BaseType>
template<class SubType>
SubType& Scope<BaseType>::Bind(Hash key)
{
	assert(std::find(mKeys.begin(), mKeys.end(), key) == mKeys.end() && "Variable already bound in this scope");

	mKeys.push_back(key);
	if(mSibling && mSibling->Has(key))
		mValues.emplace_back(new SubType(mSibling->Get<SubType>(key)));
	else if(mParent && mParent->Has(key))
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
	if(mSibling && mSibling->Has(key))
		return mSibling->Get<SubType>(key);
	if(mParent)
		return mParent->Get<SubType>(key);
	else
		throw std::runtime_error("Variable not declared");
}

template<class BaseType>
template<class SubType>
void Scope<BaseType>::Set(Hash key, SubType&& value)
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

template<class BaseType>
template<class SubType>
void Scope<BaseType>::Set(Hash key, const SubType& value)
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

template<class BaseType>
void Scope<BaseType>::Unset(Hash key)
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

template<class BaseType>
bool Scope<BaseType>::Has(Hash key) const
{
	auto it = std::find(mKeys.begin(), mKeys.end(), key);
	if(it != mKeys.end())
	{
		if(*it)
			return true;
		else
			return false;
	}
	if(mSibling && mSibling->Has(key))
		return true;
	else if(mParent)
		return mParent->Has(key);
	else
		return false;
}

template<class BaseType>
std::map<Hash, BaseType*> Scope<BaseType>::ToMap() const
{
	std::map<Hash, BaseType*> out;
	if(mParent)
		out = mParent->ToMap();
	if(mSibling)
	{
		auto siblingMap = mSibling->ToMap();
		out.insert(siblingMap.begin(), siblingMap.end());
	}
	for(size_t i = 0; i < mKeys.size(); ++i)
	{
		if(mValues[i])
			out[mKeys[i]] = mValues[i].get();
		else
			out.erase(mKeys[i]); // Erase variable from parent scope if it was Unset() in this scope
	}
	return out;
}

}
}

#endif // MOLECULAR_UTIL_SCOPE_H
