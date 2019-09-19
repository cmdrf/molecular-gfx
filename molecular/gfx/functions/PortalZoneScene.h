/*	PortalZoneScene.h
	Copyright 2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PORTALZONESCENE_H
#define PORTALZONESCENE_H

#include "gfx/RenderFunction.h"
#include "util/Matrix4.h"
#include "util/Frustum.h"
#include "DrawDebugMesh.h"

namespace Gfx
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
	AxisAlignedBox GetBounds() const override;
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
		AxisAlignedBox bounds;
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

#endif // PORTALZONESCENE_H
