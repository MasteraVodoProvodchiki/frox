#include "AstraDevice.h"
#include "AstraInspector.h"

namespace frox {

AstraDevice::AstraDevice(void* hDevice)
	: _hDevice(hDevice)
{}

AstraDevice::~AstraDevice()
{
	_sensors.clear();

	// close the Sensor
}

SensorInspectorPtr AstraDevice::CreateInpector(EInspectorType type)
{
	auto inspector = AstraInspector::Create(this->GetPtr(), type);
	inspector->Start();
	return inspector;
}

void AstraDevice::QueryData()
{
	_sensors.clear();

	// Read sensors

	for (auto sensor : _sensors)
	{
		sensor->QueryData();
	}
}

bool AstraDevice::FindProfile(AstraStreamProfile profile, AstraStreamProfile& out) const
{
	for (auto sensor : _sensors)
	{
		if (sensor->FindProfile(profile, out))
		{
			return true;
		}
	}

	return false;
}

bool AstraDevice::SupportsProfile(AstraStreamProfile Profile) const
{
	for (auto sensor : _sensors)
	{
		if (sensor->SupportsProfile(Profile))
		{
			return true;
		}
	}

	return false;
}

AstraSensorPtr AstraDevice::NewSensor(void* handle, const char* name)
{
	AstraSensorPtr sensor = AstraSensor::Create(handle);
	return sensor;
}

} // End frox.
