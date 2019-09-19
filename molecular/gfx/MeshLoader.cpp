/*	MeshLoader.cpp
	Copyright 2012-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "gfx/MeshLoader.h"

Hash MakeHash(const MeshLocator& locator)
{
	Hash hash = locator.meshFile;
	for(auto& target: locator.morphTargets)
	{
		hash = HashUtils::Combine(hash, HashUtils::MakeHash(target.file));
		hash = HashUtils::Combine(hash, HashUtils::MakeHash(reinterpret_cast<const char*>(&target.weight), sizeof(float)));
	}
	return hash;
}
