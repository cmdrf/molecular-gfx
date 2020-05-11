/*	ShadowMapping.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#ifndef MOLECULAR_SHADOWMAPPING_H
#define MOLECULAR_SHADOWMAPPING_H

#include <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace gfx
{

/// Example shadow mapping function
/** Calls the callee twice: Once for the shadow pass and once for normal render pass.

	Not view-dependent. Most parameters are currently hard-coded.
	@todo Only implemented for directional lights, no point or spot lights. */
class ShadowMapping : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	ShadowMapping(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mShadowMap(nullptr),
		mRenderTarget(nullptr)
	{
		mShadowMap = mRenderer.CreateTexture();
		mShadowMap->Store(1024, 1024, nullptr, PF_DEPTH_24);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinear);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kCompareFunc, RenderCmdSink::Texture::kLessEqual);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kCompareMode, RenderCmdSink::Texture::kCompareRefToTexture);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kWrapS, RenderCmdSink::Texture::kClampToEdge);
		mShadowMap->SetParameter(RenderCmdSink::Texture::kWrapT, RenderCmdSink::Texture::kClampToEdge);

		mRenderTarget = mRenderer.CreateRenderTarget();
		mRenderTarget->AttachDepthBuffer(mShadowMap);
	}

	~ShadowMapping() override;

protected:
	void HandleExecute(Scope& scope) override;

private:
	RenderCmdSink::Texture* mShadowMap;
	RenderCmdSink::RenderTarget* mRenderTarget;

	/// Transforms Normalized Device Coordinates (-1 to 1) to UVW (0 to 1) coordinates
	static const float kBiasMatrix[16];
};

}
}

#endif // SHADOWMAPPING_H
