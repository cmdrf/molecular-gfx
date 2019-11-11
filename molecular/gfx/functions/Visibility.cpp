#include "Visibility.h"

namespace molecular
{
namespace gfx
{


void molecular::gfx::Visibility::Execute()
{
	if(mCallee && mVisible)
		mCallee->Execute();
}

void Visibility::SetVisibility(bool visible)
{
	mVisible = visible;
}

}// namespace gfx
}// namespace molecular
