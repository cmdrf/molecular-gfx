#include "Demultiplexer.h"

namespace molecular
{
namespace gfx
{

void Demultiplexer::Execute()
{
	for(auto& func: mCallees)
		func->Execute();
}


} // namespace gfx
} // namespace molecular
