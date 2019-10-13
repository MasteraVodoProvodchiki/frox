#include "MockSensorInspector.h"

#include <Frox.h>
#include <Log.h>
#include <Utils.h>

namespace frox {

MockSensorInspector::MockSensorInspector(MockSensorDevicePtr device, EInspectorType type)
	: _device(device)
	, _type(type)
{
	assert(_device != nullptr);
}

MockSensorInspector::~MockSensorInspector()
{
	Stop();
}

bool MockSensorInspector::Start()
{
	return true;
}

void MockSensorInspector::Stop()
{
	_frame = nullptr;
}

bool MockSensorInspector::IsValid() const
{
	return true;
}

ComputeFramePtr MockSensorInspector::ReadFrame() const
{
	// Read frame from device
	auto deviceFrame = _device->GetFrame(_type);
	if (deviceFrame != nullptr)
	{
		return deviceFrame;
	}

	// Generate new frame or return from cache
	if (_frame != nullptr)
	{
		return _frame;
	}

	auto newFrame =  GenerateFrame();

	// Save cache
	_frame = newFrame;
	return newFrame;
}

ComputeFramePtr MockSensorInspector::GenerateFrame() const
{
	ComputeFramePtr frame;
	switch (_type)
	{
	case EInspectorType::Depth: {
		frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, EComputeFrameType::ECFT_UInt16);
		utils::Fill<uint16_t>(frame, [](uint16_t) -> uint16_t {
			return uint16_t(std::rand() % std::numeric_limits<uint16_t>::max());
		});
		break;
	}
	case EInspectorType::Color: {
		frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, ComputeFrameType{ EComputeFrameType::ECFT_UInt8 , 4 });
		utils::Fill<uint8_t>(frame, [](uint8_t) -> uint8_t {
			return uint8_t(std::rand() % std::numeric_limits<uint8_t>::max());
		});
		break;
	}
	case EInspectorType::Infrared: {
		frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, EComputeFrameType::ECFT_UInt8);
		utils::Fill<uint8_t>(frame, [](uint8_t) -> uint8_t {
			return uint8_t(std::rand() % std::numeric_limits<uint8_t>::max());
		});
		break;
	}
	default:
		// TODO. Something bad
		assert(false);
	}

	return frame;
}

} // End frox.
