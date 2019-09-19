/*	RequestOutput.h
	Copyright 2014-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef REQUESTOUTPUT_H
#define REQUESTOUTPUT_H

#include <molecular/gfx/RenderFunction.h>

namespace Gfx
{

/// Declare an output variable that influences shader program generation
/** Warning: ShadowMapping sets its own outputs.
	@see Output */
class RequestOutput : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	RequestOutput(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mOutput(HashUtils::MakeHash("fragmentColor"))
	{
	}

	void Execute() override;

	void SetOutput(Hash output) {mOutput = output;}
	void SetOutput(const char* name) {mOutput = HashUtils::MakeHash(name);}

private:
	Hash mOutput;
};

}

#endif // REQUESTFRAGMENTOUTPUT_H
