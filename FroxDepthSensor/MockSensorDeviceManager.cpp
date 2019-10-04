#include "MockSensorDeviceManager.h"

namespace frox {

MockSensorDeviceManager::MockSensorDeviceManager()
{}

MockSensorDeviceManager::~MockSensorDeviceManager()
{}

uint32_t MockSensorDeviceManager::GetNumDevices() const
{
	return _devices.size();
}

SensorDevice* MockSensorDeviceManager::FindDevice(const char* serial) const
{
	for (MockSensorDevicePtr device : _devices)
	{
		bool equal = (std::strcmp(device->GetSerial(), serial) == 0);
		if (equal)
		{
			return device.get();
		}
	}
	
	return nullptr;
}

SensorDevice* MockSensorDeviceManager::GetDefaultDevice() const
{
	if (!_devices.empty())
	{
		return _devices.front().get();
	}

	return nullptr;
}

MockSensorDevicePtr MockSensorDeviceManager::CreateDevice()
{
	auto device = MockSensorDevice::Create("MockSensorDevice", "fakeserial");
	_devices.push_back(device);
	return device;
}

void MockSensorDeviceManager::ClearDevices()
{
	_devices.clear();
}

} // End frox