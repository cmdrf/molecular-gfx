/*	Picking.h

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MOLECULAR_PICKING_H
#define MOLECULAR_PICKING_H

#include <molecular/util/Vector3.h>
#include "RenderManager.h"
#include <molecular/gfx/functions/RequestOutput.h>

namespace molecular
{
namespace gfx
{
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
	typename TRenderManager::RenderCmdSink& renderer = renderManager.GetRenderer();
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
}
}

#endif // PICKING_H

