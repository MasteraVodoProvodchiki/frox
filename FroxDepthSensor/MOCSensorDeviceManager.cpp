#include "MOCSensorDeviceManager.h"

namespace frox {

MOCSensorDeviceManager::MOCSensorDeviceManager()
{}

MOCSensorDeviceManager::~MOCSensorDeviceManager()
{}

uint32_t MOCSensorDeviceManager::GetNumDevices() const
{
	return _devices.size();
}

SensorDevice* MOCSensorDeviceManager::FindDevice(const char* serial) const
{
	for (MOCSensorDevicePtr device : _devices)
	{
		bool equal = (std::strcmp(device->GetSerial(), serial) == 0);
		if (equal)
		{
			return device.get();
		}
	}
	
	return nullptr;
}

SensorDevice* MOCSensorDeviceManager::GetDefaultDevice() const
{
	if (!_devices.empty())
	{
		return _devices.front().get();
	}

	return nullptr;
}

MOCSensorDevicePtr MOCSensorDeviceManager::CreateDevice()
{
	auto device = MOCSensorDevice::Create("MOCSensorDevice", "fakeserial");
	_devices.push_back(device);
	return device;
}

void MOCSensorDeviceManager::ClearDevices()
{
	_devices.clear();
}

} // End frox