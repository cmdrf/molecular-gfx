/*	Transform.cpp
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "Transform.h"
#include <molecular/gfx/DefaultProgramData.h>
#include <molecular/util/Box.h>

namespace molecular
{
namespace gfx
{

void Transform::Execute()
{
	if(mCallee)
	{
		Binding<Uniform<Matrix4> > modelMatrix("modelMatrix"_H, this);

		**modelMatrix *= mTransform;
		mCallee->Execute();
	}
}

util::AxisAlignedBox Transform::GetBounds() const
{
	util::Box box(mCallee->GetBounds());
	box.Transform(mTransform);
	return box.ToAxisAligned();
}

bool Transform::BoundsChangedSince(int framecounter) const
{
	return mCallee && (mBoundsChangedFramecounter > framecounter || mCallee->BoundsChangedSince(framecounter));
}

}
}
