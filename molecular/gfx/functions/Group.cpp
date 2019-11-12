#include "Group.h"

namespace molecular
{
namespace gfx
{

void Group::Execute()
{
	for(auto& func: mCallees)
		func->Execute();
}


} // namespace gfx
} // namespace molecular
