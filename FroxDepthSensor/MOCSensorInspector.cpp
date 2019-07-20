#include "MOCSensorInspector.h"

#include <Frox.h>
#include <Log.h>
#include <Utils.h>

namespace frox {

MOCSensorInspector::MOCSensorInspector(MOCSensorDevicePtr device, EInspectorType type)
	: _device(device)
	, _type(type)
{}

MOCSensorInspector::~MOCSensorInspector()
{
	Stop();
}

bool MOCSensorInspector::Start()
{
	switch (_type)
	{
	case EInspectorType::Depth: {
		_frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, EComputeFrameType::ECFT_UInt16);
		utils::Fill<uint16_t>(_frame, [](uint16_t) -> uint16_t {
			return uint16_t(std::rand() % std::numeric_limits<uint16_t>::max());
		});
		break;
	}
	case EInspectorType::Color: {
		_frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, ComputeFrameType{ EComputeFrameType::ECFT_UInt8 , 4});
		utils::Fill<uint8_t>(_frame, [](uint8_t) -> uint8_t {
			return uint8_t(std::rand() % std::numeric_limits<uint8_t>::max());
		});
		break;
	}
	case EInspectorType::Infrared: {
		_frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, EComputeFrameType::ECFT_UInt8);
		utils::Fill<uint8_t>(_frame, [](uint8_t) -> uint8_t {
			return uint8_t(std::rand() % std::numeric_limits<uint8_t>::max());
		});
		break;
	}
	default:
		break;
	}
	
	return true;
}

void MOCSensorInspector::Stop()
{
	_frame = nullptr;
}

bool MOCSensorInspector::IsValid() const
{
	return _frame != nullptr;
}

ComputeFramePtr MOCSensorInspector::ReadFrame() const
{
	return _frame;
}

} // End frox.
