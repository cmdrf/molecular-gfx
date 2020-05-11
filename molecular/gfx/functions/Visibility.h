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

	void SetVisibility(bool visible);

protected:
	void HandleExecute(Scope& scope) override;

private:
	bool mVisible = true;
};

}

}
#endif // MOLECULAR_VISIBILITY_H
