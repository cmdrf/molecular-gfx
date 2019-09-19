#ifndef FULLSCREENEFFECT_H
#define FULLSCREENEFFECT_H

#include <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace Gfx
{

class FullscreenEffect : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	FullscreenEffect(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager)
	{}

	void Execute() override;

private:
	RenderCmdSink::RenderTarget* mRenderTarget;
};

}
}

#endif // FULLSCREENEFFECT_H
