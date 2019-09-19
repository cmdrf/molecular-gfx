/*	NonCopyable.h
	Copyright 2011-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

/// Base class that prevents copying any object of a derived class
class NonCopyable
{
public:
	NonCopyable() = default;

	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};

class MovableOnly
{
public:
	MovableOnly() = default;

	MovableOnly(const MovableOnly&) = delete;
	MovableOnly(MovableOnly&&) = default;
	MovableOnly& operator=(const MovableOnly&) = delete;
	MovableOnly& operator=(MovableOnly&&) = default;

};

#endif
