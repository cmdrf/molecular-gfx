/*	PortalZoneScene.h

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

#ifndef MOLECULAR_PORTALZONESCENE_H
#define MOLECULAR_PORTALZONESCENE_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Matrix4.h>
#include <molecular/util/Frustum.h>
#include "DrawDebugMesh.h"

namespace molecular
{
namespace gfx
{

template<class TRenderManager, class TZoneKey>
class PortalZoneScene : public RenderFunction
{
public:
	using RenderManager = TRenderManager;
	using Leaf = RenderFunction*;
	using ZoneKey = TZoneKey;

	explicit PortalZoneScene(RenderManager& renderManager) :
	    RenderFunction(renderManager),
	    mDebugMesh(renderManager)
	{

	}

	void Execute() override;
	util::AxisAlignedBox GetBounds() const override;
	bool BoundsChangedSince(int framecounter) const override;

	Leaf Insert(RenderFunction* function);
	Leaf Insert(ZoneKey zone, RenderFunction* function);
	void Remove(Leaf leaf);

	void InsertZone(ZoneKey key, RenderFunction* sky = nullptr);
	void EraseZone(ZoneKey zone);

	template<class PointIterator>
	void InsertPortal(ZoneKey zone, PointIterator pointsBegin, PointIterator pointsEnd, ZoneKey destinationZone);

private:
	struct Portal
	{
		static const unsigned int kMaxPoints = 8;
		Vector3 points[kMaxPoints];
		unsigned int numPoints;
		ZoneKey destinationZone;
	};

	struct Zone : public MovableOnly
	{
		static const unsigned int kMaxPortals = 8;
		Portal portals[kMaxPortals];
		unsigned int numPortals = 0;
		RenderFunction* sky = nullptr;
		util::AxisAlignedBox bounds;
		std::unordered_set<RenderFunction*> functions;
	};
	using ZoneMap = std::unordered_map<ZoneKey, Zone>;

	typename ZoneMap::iterator FindZone(const Vector3& position);
	typename ZoneMap::iterator FindZone(const Vector3& position, typename ZoneMap::iterator hint);

	template<class PlaneIterator>
	void DrawZone(Zone& zone, PlaneIterator planesBegin, PlaneIterator planesEnd, const Vector3& viewPos);

	/// Functions not belonging to any zone
	std::unordered_set<RenderFunction*> mFunctions;

	ZoneMap mZones;
	typename ZoneMap::iterator mCurrentViewZone = mZones.end();
	DrawDebugMesh mDebugMesh;
};

}
}

#endif // PORTALZONESCENE_H
