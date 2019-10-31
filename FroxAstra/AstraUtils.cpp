#include "AstraUtils.h"
#include <Frox.h>

namespace frox {

ComputeFramePtr CreateFrame(astra::DepthStream depthStream)
{
	if (!depthStream.is_available())
	{
		return nullptr;
	}

	// Read Info
	auto mode = depthStream.mode();
	uint32_t width = mode.width();
	uint32_t height = mode.height();
	uint8_t bytesPerPixel = mode.bytes_per_pixel();
	assert(bytesPerPixel == 2);

	// Create frame
	return FroxInstance()->CreateComputeFrame(Size{ width, height }, ComputeFrameType{ EComputeFrameType::ECFT_UInt16, 1 });
}

ComputeFramePtr CreateFrame(astra::ColorStream colorStream)
{
	if (!colorStream.is_available())
	{
		return nullptr;
	}

	// Read Info
	auto mode = colorStream.mode();
	uint32_t width = mode.width();
	uint32_t height = mode.height();
	uint8_t bytesPerPixel = mode.bytes_per_pixel();
	assert(bytesPerPixel == 3);

	// Create frame
	return FroxInstance()->CreateComputeFrame(Size{ width, height }, ComputeFrameType{ EComputeFrameType::ECFT_UInt8, 3 });
}

ComputeFramePtr CreateFrame(astra::InfraredStream infraredStream)
{
	if (!infraredStream.is_available())
	{
		return nullptr;
	}

	// Read Info
	auto mode = infraredStream.mode();
	uint32_t width = mode.width();
	uint32_t height = mode.height();
	uint8_t bytesPerPixel = mode.bytes_per_pixel();
	assert(bytesPerPixel == 2);

	// Create frame
	return FroxInstance()->CreateComputeFrame(Size{ width, height }, ComputeFrameType{ EComputeFrameType::ECFT_UInt16, 1 });
}

} // End frox.