#include "MockSensorDevice.h"
#include "MockSensorInspector.h"

#include <assert.h>

namespace frox {

MockSensorDevice::MockSensorDevice(const char* name, const char* serial)
	: _name(name)
	, _serial(serial)
{}

MockSensorDevice::~MockSensorDevice()
{}

// SensorDevice overrides
const char* MockSensorDevice::GetName() const
{
	return _name.data();
}

const char* MockSensorDevice::GetSerial() const
{
	return _serial.data();
}

SensorInspectorPtr MockSensorDevice::CreateInpector(EInspectorType type)
{
	auto inspector = MockSensorInspector::Create(this->GetPtr(), type);
	inspector->Start();
	return inspector;
}

void MockSensorDevice::MapDepthFrameToColorFrame(ComputeFramePtr depthFrame, ComputeFramePtr colorFrame, ComputeFramePtr output)
{
}

void MockSensorDevice::SetDepthFrame(ComputeFramePtr frame)
{
	_depthFrame = frame;
}

void MockSensorDevice::SetColorFrame(ComputeFramePtr frame)
{
	_colorFrame = frame;
}

void MockSensorDevice::SetInfraredFrame(ComputeFramePtr frame)
{
	_infraredFrame = frame;
}

ComputeFramePtr MockSensorDevice::GetFrame(EInspectorType type) const
{
	switch (type)
	{
	case frox::EInspectorType::Depth:
		return _depthFrame;
	case frox::EInspectorType::Color:
		return _colorFrame;
	case frox::EInspectorType::Infrared:
		return _infraredFrame;
	default:
		assert(false);
	}

	return nullptr;
}

} // End frox