/*	Skeleton.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef SKELETON_H
#define SKELETON_H

#include "gfx/RenderFunction.h"
#include "CharacterAnimation.h"

namespace Gfx
{

class Skeleton : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	Skeleton(TRenderManager& manager);

	void Execute() override;
	AxisAlignedBox GetBounds() const override {return mCallee->GetBounds();}

	void SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount]);
	void SetBindPose(const Matrix4 inverseBindPoseMatrices[CharacterAnimation::kBoneCount]);

private:
	bool mActive = false;
	Matrix4 mJoints[CharacterAnimation::kBoneCount];
	Matrix4 mBindPose[CharacterAnimation::kBoneCount];
};

template<class TRenderManager>
Skeleton::Skeleton(TRenderManager& manager) :
    SingleCalleeRenderFunction(manager)
{

}

}

#endif // SKELETON_H
