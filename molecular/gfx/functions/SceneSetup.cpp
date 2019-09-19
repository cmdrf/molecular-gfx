/*	SceneSetup.cpp
	Copyright 2012-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "SceneSetup.h"

namespace molecular
{
namespace Gfx
{

void SceneSetup::Execute()
{
	Binding<Uniform<Hash>> blendMode("blendMode"_H, this);
	**blendMode = "none"_H;

	if(mDirectionalLightEnabled)
	{
		Binding<Uniform<Vector3> > lightDirection0("lightDirection0"_H, this);
		Binding<Uniform<Vector3> > lightColor0("lightColor0"_H, this);

		**lightDirection0 = mLightDirection;
		**lightColor0 = mLightColor;

		mCallee->Execute();
	}
	else
		mCallee->Execute();
}

void SceneSetup::SetDirectionalLight(bool enable, const Vector3& direction, const Vector3& color)
{
	mDirectionalLightEnabled = enable;
	mLightDirection = direction.Normalized();
	mLightColor = color;
}

void SceneSetup::Set(Hash key, bool value)
{
	if(key == "directionalLightEnabled"_H)
		mDirectionalLightEnabled = value;
	else
		throw std::runtime_error("Unknown bool variable in SceneSetup");
}

}
}
