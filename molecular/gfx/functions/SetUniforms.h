/*	SetUniforms.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef SETUNIFORMS_H
#define SETUNIFORMS_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/Material.h>

namespace molecular
{
namespace Gfx
{

class SetUniforms : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	SetUniforms(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mUniforms(manager)
	{
	}

	void Execute() override;

	template<class T>
	void SetUniform(Hash hash, const T& variable)
	{
		mUniforms.SetUniform(hash, variable);
	}

private:
	Material mUniforms;
};

}
}

#endif // SETUNIFORMS_H
