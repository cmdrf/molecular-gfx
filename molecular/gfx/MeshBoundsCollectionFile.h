/*	MeshBoundsCollectionFile.h

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

#ifndef MOLECULAR_MESHBOUNDSCOLLECTIONFILE_H
#define MOLECULAR_MESHBOUNDSCOLLECTIONFILE_H

#include <molecular/util/AxisAlignedBox.h>
#include <molecular/util/Hash.h>

namespace molecular
{

static_assert(sizeof(util::AxisAlignedBox) == 6 * sizeof(float), "AxisAlignedBox does not only contain 6 floats");

struct MeshBoundsCollectionFile
{
	static const uint32_t kMagic = 0x505eb0f1;
	uint32_t magic;
	uint32_t count;
	uint32_t hashes[0];

	const util::AxisAlignedBox& GetBoundsByIndex(unsigned int index) const
	{
		auto boxes = reinterpret_cast<const util::AxisAlignedBox*>(reinterpret_cast<const uint8_t*>(this) +  count * sizeof(Hash) + sizeof(MeshBoundsCollectionFile));
		return boxes[index];
	}

	const util::AxisAlignedBox& GetBounds(Hash mesh) const
	{
		for(unsigned int i = 0; i < count; ++i)
		{
			if(hashes[i] == mesh)
				return GetBoundsByIndex(i);
		}

		return util::AxisAlignedBox::kDefault;
	}
};

}

#endif // MESHBOUNDSCOLLECTIONFILE_H
