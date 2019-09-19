/*	Transform.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace Gfx
{

class Transform : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	Transform(TRenderManager &manager) :
		SingleCalleeRenderFunction(manager),
		mBoundsChangedFramecounter(manager.GetFramecounter())
	{
	}

	void Execute() override;
	molecular::util::AxisAlignedBox GetBounds() const override;
	bool BoundsChangedSince(int framecounter) const override;

	void SetTransform(const Matrix4& transform)
	{
		mTransform = transform;
//		mBoundsChangedFramecounter = mRenderManager.GetFramecounter();
	}


private:
	Matrix4 mTransform;
	int mBoundsChangedFramecounter;
};

}
}

#endif // TRANSFORM_H
