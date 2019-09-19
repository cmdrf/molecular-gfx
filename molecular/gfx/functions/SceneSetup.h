/*	SceneSetup.h
	Copyright 2012-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef SCENESETUP_H
#define SCENESETUP_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Quaternion.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace Gfx
{

/// Sets lighting and view and projection matrices
/** @todo Don't interfere with projection matrix from RenderContext. */
class SceneSetup : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	SceneSetup(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager)
	{
	}

	void Execute() override;

	/// Set light parameters
	void SetDirectionalLight(bool enable, const Vector3& direction = Vector3(1,-1,-1), const Vector3 &color = Vector3(1,1,1));

	void Set(Hash key, bool value) override;

private:
	Vector3 mLightDirection = Vector3(1,-1,-1);
	Vector3 mLightColor = Vector3(1,1,1);
	bool mDirectionalLightEnabled = true;
};

}
}

#endif // SCENESETUP_H
