#ifndef MOLECULAR_VISIBILITY_H
#define MOLECULAR_VISIBILITY_H

#include <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace gfx
{


class Visibility : public SingleCalleeRenderFunction
{
public:
	template <typename TRenderManager>
	Visibility(TRenderManager& manager):
		SingleCalleeRenderFunction(manager) {}

	void Execute() override;
	void SetVisibility(bool visible);

private:
	bool mVisible = true;
};

}

}
#endif // MOLECULAR_VISIBILITY_H
