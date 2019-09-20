#ifndef MOLECULAR_POINTERCONTAINER_H
#define MOLECULAR_POINTERCONTAINER_H

#include <vector>
#include <map>
#include <list>
#include "NonCopyable.h"

namespace molecular
{

template<class T>
class PointerContainer : public T, NonCopyable
{
public:
	~PointerContainer()
	{
		typename T::iterator it;
		for(it = this->begin(); it != this->end(); ++it)
			delete *it;
	}
};

template<class T>
class PointerVector : public PointerContainer<std::vector<T*> >
{};

template <class S, class T>
class PointerMap : public std::map<S, T*>, NonCopyable
{
public:
	~PointerMap()
	{
		typename std::map<S, T*>::iterator it;
		for(it = this->begin(); it != this->end(); ++it)
			delete it->second;
	}
};

template <class T>
class PointerList : public PointerContainer<std::list<T*> >
{};

}
#endif // MOLECULAR_POINTERCONTAINER_H