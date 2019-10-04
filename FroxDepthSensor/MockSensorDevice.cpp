#include "MockSensorDevice.h"
#include "MockSensorInspector.h"

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

} // End frox