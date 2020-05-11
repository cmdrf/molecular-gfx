#include "Group.h"

namespace molecular
{
namespace gfx
{

void Group::HandleExecute(Scope& scope)
{
	for(auto& func: mCallees)
		func->Execute(&scope);
}


} // namespace gfx
} // namespace molecular
