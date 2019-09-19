/*	RenderCmdSink.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef RENDERCMDSINK_H
#define RENDERCMDSINK_H

#include <molecular/gfx/Config.h>

#if MOLECULAR_ENABLE_VULKAN
#include "vulkan/VulkanRenderCmdSink.h"
#include "vulkan/VulkanRenderCmdSinkProgram.h"

namespace molecular
{
namespace Gfx
{

using RenderCmdSink = VulkanRenderCmdSink;

}
}

#else
#include "opengl/GlCommandSink.h"
#include "opengl/GlCommandSinkProgram.h"

namespace molecular
{
namespace Gfx
{

using RenderCmdSink = GlCommandSink;

}
}
#endif

#endif // RENDERCMDSINK_H
