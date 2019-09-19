/*	MeshLocator.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MESHLOCATOR_H
#define MESHLOCATOR_H

#include <vector>
#include <string>
#include "util/Hash.h"

/// Specifies a mesh file and optional morph targets
struct MeshLocator
{
	/// Mesh file path
	Hash meshFile;

	/// Specifies a morph target file path and its weight
	struct MorphTarget
	{
		/// Morph target file name
		std::string file;

		/// Morph target weight
		/** From 0 to 1. */
		float weight;
	};
	std::vector<MorphTarget> morphTargets;
};


#endif // MESHLOCATOR_H

