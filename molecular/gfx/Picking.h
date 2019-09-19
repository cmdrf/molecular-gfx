/*	Picking.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PICKING_H
#define PICKING_H

#include "util/Vector3.h"
#include "RenderManager.h"
#include "functions/RequestOutput.h"

namespace Picking
{
/// Return value of DoPicking()
struct Result
{
	Vector3 coords;
	unsigned int pickedId;
};

/** @param mouseY Y coordinate, counting from the upper edge */
template<class TRenderManager>
Result DoPicking(TRenderManager& renderManager, RenderFunction& function, int mouseX, int mouseY)
{
	Renderer& renderer = renderManager.GetRenderer();
	int width = 0;
	int height = 0;
	renderer.GetViewport(width, height);

	const PixelFormat colorFormat = PF_L32_UNSIGNED; //PF_R8G8B8A8_UNSIGNED;
	const PixelFormat depthFormat = PF_DEPTH_32;

	RequestOutput requestOutput(renderManager);
	requestOutput.SetOutput("pickingOutput"_H);
	requestOutput.SetCallee(&function);

	// Create RenderTarget to draw picking colors to:
	// TODO: Call BeginDraw first?
	auto target = renderer.CreateRenderTarget();
	target->AttachColorBuffer(width, height, colorFormat);
	target->AttachDepthBuffer(width, height, depthFormat);

	// Draw:
	renderer.BeginDraw();
	renderer.SetTarget(target);
	renderer.SetBackgroundColor(0, 0, 0, 0);
	renderer.Clear(true, true);
	requestOutput.Execute();

	// Read pixels from rendertarget:
	uint32_t color = 0;
	uint32_t depth = 0;
	renderer.ReadPixels(mouseX, height - mouseY, 1, 1, colorFormat, &color);
	renderer.ReadPixels(mouseX, height - mouseY, 1, 1, depthFormat, &depth);

	// Cleanup:
	renderer.FinishDraw();
	renderer.DestroyRenderTarget(target);

	float x = float(mouseX) / width;
	float y = float(mouseY) / height;
	float z = float(depth) / std::numeric_limits<decltype(depth)>::max();

	Result result;
	result.coords = Vector3(x, y, z) * 2.0f - Vector3(1,1,1);
	result.pickedId = color;
	return result;
}
}

#endif // PICKING_H

