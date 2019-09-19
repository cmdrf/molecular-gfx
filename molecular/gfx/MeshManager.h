/*	MeshManager.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFX_MESHMANAGER_H
#define GFX_MESHMANAGER_H

#include "AssetManager.h"
#include "MeshLocator.h"

/// Modified hashing function that hashes MeshLocator
/** Includes the file name and morph targets with their respective weights. */
Hash MakeHash(const MeshLocator& locator);

namespace Gfx
{

class DrawMeshData;

typedef AssetManager<DrawMeshData*, 1, false, MeshLocator> MeshManager;

}

#endif // GFX_MESHMANAGER_H

