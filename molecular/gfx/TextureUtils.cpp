#define NOMINMAX
#include <algorithm>
#include "TextureUtils.h"

namespace molecular
{
namespace Gfx
{
namespace TextureUtils
{

RenderCmdSink::Texture* Random1DTexture(RenderCmdSink& renderer, unsigned int extent)
{
	std::vector<float> pRaw(extent * 4);

	for(unsigned int n = 0; n < extent * 4; n++)
		pRaw[n] = rand() / (float)RAND_MAX;

	RenderCmdSink::Texture* out = renderer.CreateTexture();
	out->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);
	out->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinear);
	out->SetParameter(RenderCmdSink::Texture::kWrapS, RenderCmdSink::Texture::kRepeat);
	out->SetParameter(RenderCmdSink::Texture::kWrapT, RenderCmdSink::Texture::kRepeat);
	out->Store(extent, 1, pRaw.data(), PF_RGBA_FLOAT32);

	return out;
}

RenderCmdSink::Texture* SpotTexture(RenderCmdSink& renderer, unsigned int width, unsigned int height)
{
	std::vector<uint8_t> pRaw(width * height);

	const float fMaxDist = std::sqrt(width * height / 2.f);
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			const float length = std::sqrt((x - width * 0.5f) * (x - width * 0.5f) + (y - height * 0.5f) * (y - height * 0.5f));
			const float fSpot  = std::min(std::pow((fMaxDist - length) / fMaxDist, 8.f), 1.0f);

			pRaw[x + y * width] = fSpot * 255.0f > 0 ? fSpot * 255.0f : 0;
		}
	}

	RenderCmdSink::Texture* out = renderer.CreateTexture();
	out->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);
	out->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinearMipmapLinear);
	out->SetParameter(RenderCmdSink::Texture::kWrapS, RenderCmdSink::Texture::kClampToEdge);
	out->SetParameter(RenderCmdSink::Texture::kWrapT, RenderCmdSink::Texture::kClampToEdge);
	out->Store(width, height, pRaw.data(), PF_A8);
	out->GenerateMipmaps();

	return out;
}

}
}
}
