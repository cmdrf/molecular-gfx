/*	MeshBoundsCollectionFile.h
	Copyright 2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MESHBOUNDSCOLLECTIONFILE_H
#define MESHBOUNDSCOLLECTIONFILE_H

#include "util/AxisAlignedBox.h"
#include "util/Hash.h"

static_assert(sizeof(AxisAlignedBox) == 6 * sizeof(float), "AxisAlignedBox does not only contain 6 floats");

struct MeshBoundsCollectionFile
{
	static const uint32_t kMagic = 0x505eb0f1;
	uint32_t magic;
	uint32_t count;
	uint32_t hashes[0];

	const AxisAlignedBox& GetBoundsByIndex(unsigned int index) const
	{
		auto boxes = reinterpret_cast<const AxisAlignedBox*>(reinterpret_cast<const uint8_t*>(this) +  count * sizeof(Hash) + sizeof(MeshBoundsCollectionFile));
		return boxes[index];
	}

	const AxisAlignedBox& GetBounds(Hash mesh) const
	{
		for(unsigned int i = 0; i < count; ++i)
		{
			if(hashes[i] == mesh)
				return GetBoundsByIndex(i);
		}

		return AxisAlignedBox::kDefault;
	}
};

#endif // MESHBOUNDSCOLLECTIONFILE_H
