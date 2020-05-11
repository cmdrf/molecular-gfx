#include "Visibility.h"

namespace molecular
{
namespace gfx
{

void Visibility::SetVisibility(bool visible)
{
	mVisible = visible;
}

void molecular::gfx::Visibility::HandleExecute(Scope& scope)
{
	if(mCallee && mVisible)
		mCallee->Execute(&scope);
}

}// namespace gfx
}// namespace molecular
