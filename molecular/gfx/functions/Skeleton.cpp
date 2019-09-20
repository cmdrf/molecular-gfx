/*	Skeleton.cpp
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "Skeleton.h"
#include <array>

namespace molecular
{
namespace Gfx
{

void Skeleton::Execute()
{
	if(mActive)
	{
		Binding<Uniform<Matrix4, CharacterAnimation::kBoneCount>> joints("bones"_H, this);
		for(int i = 0; i < CharacterAnimation::kBoneCount; i++)
			(*joints)[i] = mJoints[i];
		mCallee->Execute();
	}
	else
		mCallee->Execute();
}

void Skeleton::SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount])
{
	mActive = true;
	std::array<Matrix4, CharacterAnimation::kBoneCount> absoluteTransforms;
	CharacterAnimation::SceneGraphToAbsoluteTransforms(joints, CharacterAnimation::kBoneParents, absoluteTransforms.data(), CharacterAnimation::kBoneCount);
	for(int i = 0; i < CharacterAnimation::kBoneCount; i++)
	{
		mJoints[i] = absoluteTransforms[i] * mBindPose[i];
	}
}

void Skeleton::SetBindPose(const Matrix4 inverseBindPoseMatrices[CharacterAnimation::kBoneCount])
{
	mActive = true;
	std::copy(inverseBindPoseMatrices, inverseBindPoseMatrices + CharacterAnimation::kBoneCount, mBindPose);
}

}
}
