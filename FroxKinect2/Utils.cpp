#include "Utils.h"
#include <Frox.h>

#include <assert.h>

namespace frox {

ComputeFramePtr CreateDepthFrame(IFrameDescription* pFrameDescription)
{
	uint32_t bytesPerPixel;
	int32_t height, width;

	pFrameDescription->get_BytesPerPixel(&bytesPerPixel);
	pFrameDescription->get_Width(&width);
	pFrameDescription->get_Height(&height);

	assert(bytesPerPixel == sizeof(uint16_t));

	// Create frame
	return FroxInstance()->CreateComputeFrame(Size{ uint32_t(width), uint32_t(height) }, ComputeFrameType{ EComputeFrameType::ECFT_UInt16, 1 });
}

ComputeFramePtr CreateColorFrame(IFrameDescription* pFrameDescription)
{
	uint32_t bytesPerPixel;
	int32_t height, width;

	pFrameDescription->get_BytesPerPixel(&bytesPerPixel);
	pFrameDescription->get_Width(&width);
	pFrameDescription->get_Height(&height);

	assert(bytesPerPixel == sizeof(uint8_t) * 4);

	// Create frame
	return FroxInstance()->CreateComputeFrame(Size{ uint32_t(width), uint32_t(height) }, ComputeFrameType{ EComputeFrameType::ECFT_UInt8, 4 });
}

ComputeFramePtr CreateInfraredFrame(IFrameDescription* pFrameDescription)
{
	uint32_t bytesPerPixel;
	int32_t height, width;

	pFrameDescription->get_BytesPerPixel(&bytesPerPixel);
	pFrameDescription->get_Width(&width);
	pFrameDescription->get_Height(&height);

	assert(bytesPerPixel == sizeof(uint16_t));

	// Create frame
	return FroxInstance()->CreateComputeFrame(Size{ uint32_t(width), uint32_t(height) }, ComputeFrameType{ EComputeFrameType::ECFT_UInt16, 1 });
}

ComputeFramePtr CreateFrame(IDepthFrameSource* pFrameSource)
{
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->get_FrameDescription(&pFrameDescription);
	if (pFrameDescription != nullptr)
	{
		auto frame = CreateDepthFrame(pFrameDescription);
		pFrameDescription->Release();

		return frame;
	}

	return nullptr;
}

ComputeFramePtr CreateFrame(IColorFrameSource* pFrameSource)
{
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Bgra, &pFrameDescription);
	if (pFrameDescription != nullptr)
	{
		auto frame = CreateColorFrame(pFrameDescription);
		pFrameDescription->Release();

		return frame;
	}

	return nullptr;
}

ComputeFramePtr CreateFrame(IInfraredFrameSource* pFrameSource)
{
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->get_FrameDescription(&pFrameDescription);
	if (pFrameDescription != nullptr)
	{
		auto frame = CreateInfraredFrame(pFrameDescription);
		pFrameDescription->Release();

		return frame;
	}

	return nullptr;
}

} // End frox.