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
class Demultiplexer : public MultipleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	Demultiplexer(TRenderManager& manager):
		MultipleCalleeRenderFunction (manager)
	{}

	void Execute();

	/// @todo implement
	util::AxisAlignedBox GetBounds() const {return molecular::util::AxisAlignedBox(0, 0, 0, 0, 0, 0);}
	/// @todo implement
	bool BoundsChangedSince(int) const {return false;}
};



} // namespace gfx
} // namespace molecular

#endif // MOLECULAR_GFX_DEMULTIPLEXER_H
