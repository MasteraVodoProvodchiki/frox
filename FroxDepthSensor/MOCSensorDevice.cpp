#include "MOCSensorDevice.h"
#include "MOCSensorInspector.h"

namespace frox {

MOCSensorDevice::MOCSensorDevice(const char* name, const char* serial)
	: _name(name)
	, _serial(serial)
{}

MOCSensorDevice::~MOCSensorDevice()
{}

// SensorDevice overrides
const char* MOCSensorDevice::GetName() const
{
	return _name.data();
}

const char* MOCSensorDevice::GetSerial() const
{
	return _serial.data();
}

SensorInspectorPtr MOCSensorDevice::CreateInpector(EInspectorType type)
{
	auto inspector = MOCSensorInspector::Create(this->GetPtr(), type);
	inspector->Start();
	return inspector;
}

} // End frox