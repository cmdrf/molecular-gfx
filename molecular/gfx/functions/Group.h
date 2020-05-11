#ifndef MOLECULAR_GFX_DEMULTIPLEXER_H
#define MOLECULAR_GFX_DEMULTIPLEXER_H

#include <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace gfx
{

/**
 * @brief The Demultiplexer class is a utility class for cases where more than one output is required.
 */
class Group : public MultipleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	Group(TRenderManager& manager):
		MultipleCalleeRenderFunction (manager)
	{}

protected:
	void HandleExecute(Scope& scope) override;
};



} // namespace gfx
} // namespace molecular

#endif // MOLECULAR_GFX_DEMULTIPLEXER_H
