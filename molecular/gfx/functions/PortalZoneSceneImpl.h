/*	PortalZoneSceneImpl.h
	Copyright 2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PORTALZONESCENEIMPL_H
#define PORTALZONESCENEIMPL_H

#include "PortalZoneScene.h"
#include "util/PlaneSet.h"

namespace Gfx
{

template<class TRenderManager, class TZoneKey>
void PortalZoneScene<TRenderManager, TZoneKey>::Execute()
{
	const Uniform<Matrix4>* projMatrix = GetVariable<Uniform<Matrix4> >("projectionMatrix"_H);
	const Uniform<Matrix4>* viewMatrix = GetVariable<Uniform<Matrix4> >("viewMatrix"_H);

	if(!projMatrix || !viewMatrix)
		return;

	Matrix4 viewProjectionMatrix = **projMatrix * **viewMatrix;
	Frustum frustum(viewProjectionMatrix);
	auto planes = frustum.GetPlanes();

	Vector3 cameraPos = Matrix4((**viewMatrix).Inverse()).GetTranslation();
	mCurrentViewZone = FindZone(cameraPos, mCurrentViewZone);
	if(mCurrentViewZone != mZones.end())
	{
		auto& zone = mCurrentViewZone->second;
		mDebugMesh.DrawAxisAlignedBox(zone.bounds.GetMin(), zone.bounds.GetMax(), Vector3(0, 1, 0));
		DrawZone(zone, planes.begin(), planes.end(), cameraPos);
	}

	for(auto func: mFunctions)
	{
		auto bounds = func->GetBounds(); // TODO: Cache
		if(frustum.Check(bounds) != Plane::kOutside)
			func->Execute();
	}

	// Debug:
	Binding<Uniform<Matrix4>> modelMatrix("modelMatrix"_H, this);
	**modelMatrix = Matrix4::Identity();
	mDebugMesh.Execute();
}

template<class TRenderManager, class TZoneKey>
AxisAlignedBox PortalZoneScene<TRenderManager, TZoneKey>::GetBounds() const
{
	// TODO
	return AxisAlignedBox();
}

template<class TRenderManager, class TZoneKey>
bool PortalZoneScene<TRenderManager, TZoneKey>::BoundsChangedSince(int framecounter) const
{
	// TODO
	return false;
}

template<class TRenderManager, class TZoneKey>
typename PortalZoneScene<TRenderManager, TZoneKey>::Leaf
PortalZoneScene<TRenderManager, TZoneKey>::Insert(RenderFunction* function)
{
	// TODO: Find zone
	mFunctions.insert(function);
	return function;
}

template<class TRenderManager, class TZoneKey>
typename PortalZoneScene<TRenderManager, TZoneKey>::Leaf
PortalZoneScene<TRenderManager, TZoneKey>::Insert(ZoneKey zoneKey, RenderFunction* function)
{
	auto zoneIt = mZones.find(zoneKey);
	if(zoneIt == mZones.end())
		return Insert(function);
	else
	{
		auto& zone = zoneIt->second;
		zone.bounds.Stretch(function->GetBounds());
		// Add mesh to zone:
		zone.functions.insert(function);
	}
	return function;
}

template<class TRenderManager, class TZoneKey>
void PortalZoneScene<TRenderManager, TZoneKey>::Remove(Leaf leaf)
{
	for(auto& zone: mZones)
		if(zone.second.functions.erase(leaf))
			return;
	mFunctions.erase(leaf);
}

template<class TRenderManager, class TZoneKey>
void PortalZoneScene<TRenderManager, TZoneKey>::InsertZone(ZoneKey key, RenderFunction* sky)
{
	Zone& zone = mZones[key];
	zone.sky = sky;
	mCurrentViewZone = mZones.end();
}

template<class TRenderManager, class TZoneKey>
void PortalZoneScene<TRenderManager, TZoneKey>::EraseZone(ZoneKey zone)
{
	mZones.erase(zone);
	mCurrentViewZone = mZones.end();
}

template<class TRenderManager, class TZoneKey>
template<class PointIterator>
void PortalZoneScene<TRenderManager, TZoneKey>::InsertPortal(ZoneKey zoneKey, PointIterator pointsBegin, PointIterator pointsEnd, ZoneKey destinationZone)
{
	Zone& zone = mZones[zoneKey];
	if(zone.numPortals >= Zone::kMaxPortals)
		throw std::runtime_error("Maximum number of portals per zone reached");

	Portal portal;
	portal.numPoints = pointsEnd - pointsBegin;
	assert(portal.numPoints <= Portal::kMaxPoints);
	assert(portal.numPoints >= 3);
	std::copy(pointsBegin, pointsEnd, portal.points);
	portal.destinationZone = destinationZone;
	zone.portals[zone.numPortals] = portal;
	zone.numPortals++;
}

template<class TRenderManager, class TZoneKey>
typename PortalZoneScene<TRenderManager, TZoneKey>::ZoneMap::iterator
PortalZoneScene<TRenderManager, TZoneKey>::FindZone(const Vector3& position)
{
	return std::find_if(mZones.begin(), mZones.end(), [position](auto& zone){return zone.second.bounds.Contains(position);});
}

template<class TRenderManager, class TZoneKey>
typename PortalZoneScene<TRenderManager, TZoneKey>::ZoneMap::iterator
PortalZoneScene<TRenderManager, TZoneKey>::FindZone(const Vector3& position, typename ZoneMap::iterator hint)
{
	if(hint != mZones.end() && hint->second.bounds.Contains(position))
		return hint;
	return FindZone(position);
}

inline bool PortalFacingToViewer(const Vector3 portalPoints[3], const Vector3& viewPos)
{
	Vector3 kj = portalPoints[0] - portalPoints[1];
	Vector3 lj = portalPoints[0] - portalPoints[2];
	Vector3 normal = kj.CrossProduct(lj);
	return normal.DotProduct(portalPoints[0] - viewPos) < 0;
}

template<class TRenderManager, class TZoneKey>
template<class PlaneIterator>
void PortalZoneScene<TRenderManager, TZoneKey>::DrawZone(Zone& zone, PlaneIterator planesBegin, PlaneIterator planesEnd, const Vector3& viewPos)
{
	if(zone.sky)
		zone.sky->Execute();
	for(auto& func: zone.functions)
	{
		auto bounds = func->GetBounds(); // TODO: Cache
		if(PlaneSet::CheckIntersection(planesBegin, planesEnd, bounds) != Plane::kOutside)
			func->Execute();
	}

	for(unsigned int i = 0; i < zone.numPortals; ++i)
	{
		auto& portal = zone.portals[i];
		assert(portal.numPoints >= 3);
		if(!PortalFacingToViewer(portal.points, viewPos))
			continue;

		auto nextZoneIt = mZones.find(portal.destinationZone);
		if(nextZoneIt == mZones.end())
			continue;
		auto& nextZone = nextZoneIt->second;

		std::array<Vector3, 128> clippedPortal;
		auto clippedPortalEnd = PlaneSet::ClipPolygon(planesBegin, planesEnd, portal.points, portal.points + portal.numPoints, clippedPortal.begin());
		auto numPoints = clippedPortalEnd - clippedPortal.begin();
		if(numPoints == 0 || numPoints >= clippedPortal.size() - 2)
			continue;

		for(unsigned int i = 0; i < numPoints; i++)
			mDebugMesh.DrawLine(clippedPortal[i], clippedPortal[(i + 1) % numPoints], Vector3(1, 0, 0));

		std::array<Plane, 128> clippedPlanes;
		auto clippedPlanesBegin = std::copy(planesBegin, planesBegin + 2, clippedPlanes.begin()); // Copy near and far plane TODO: Use portal as near plane
		auto clippedPlanesEnd = PlaneSet::CreateCullingPlanes(viewPos, clippedPortal.begin(), clippedPortalEnd, clippedPlanesBegin);

		assert(clippedPlanesEnd - clippedPlanes.begin() == numPoints + 2);
		DrawZone(nextZone, clippedPlanes.begin(), clippedPlanesEnd, viewPos);
	}
}

}

#endif // PORTALZONESCENEIMPL_H
