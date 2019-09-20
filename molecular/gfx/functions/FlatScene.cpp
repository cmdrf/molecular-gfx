/*	FlatScene.cpp
	Copyright 2014-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "FlatScene.h"
#include <molecular/util/Matrix4.h>
#include <molecular/util/Frustum.h>
#include <molecular/util/Plane.h>

namespace molecular
{
namespace Gfx
{

void FlatScene::Execute()
{
	const Uniform<Matrix4>* projMatrix = GetVariable<Uniform<Matrix4> >("projectionMatrix"_H);
	const Uniform<Matrix4>* viewMatrix = GetVariable<Uniform<Matrix4> >("viewMatrix"_H);

	if(!projMatrix || !viewMatrix)
		return;

	Matrix4 viewProjectionMatrix = **projMatrix * **viewMatrix;
	util::Frustum frustum(viewProjectionMatrix);

	for(auto func: mFunctions)
	{
		auto bounds = func->GetBounds(); // TODO: Cache
		if(frustum.Check(bounds) != util::Plane::kOutside)
			func->Execute();
	}
}

util::AxisAlignedBox FlatScene::GetBounds() const
{
	// TODO
	return util::AxisAlignedBox();
}

bool FlatScene::BoundsChangedSince(int framecounter) const
{
	// TODO
	return false;
}

}
}
