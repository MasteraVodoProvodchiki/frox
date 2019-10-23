#include "AstraDeviceManager.h"
#include "AstraModule.h"

#include <Log.h>

namespace frox {

AstraDeviceManager::AstraDeviceManager()
{}

AstraDeviceManager::~AstraDeviceManager()
{}

uint32_t AstraDeviceManager::GetNumDevices() const
{
	return _devices.size();
}

SensorDevice* AstraDeviceManager::FindDevice(const char* serial) const
{
	AstraDevicePtr device = FindDeviceBySerial(serial);
	return device.get();
}

SensorDevice* AstraDeviceManager::GetDefaultDevice() const
{
	if (!_devices.empty())
	{
		return _devices.front().get();
	}

	return nullptr;
}

AstraDevicePtr AstraDeviceManager::NewDevice(void* handle, const char* name)
{
	auto device = AstraDevice::Create(handle);
	return device;
}

void AstraDeviceManager::QueryDevices()
{
	_devices.clear();

	// Read devices

	for (auto device : _devices)
	{
		device->QueryData();
	}
}

AstraDevicePtr AstraDeviceManager::GetDeviceById(uint32_t id) const
{
	return id < _devices.size() ? _devices[id] : nullptr;
}

AstraDevicePtr AstraDeviceManager::FindDeviceBySerial(const char* serial) const
{
	auto it = std::find_if(_devices.begin(), _devices.end(), [serial](AstraDevicePtr device) {
		return device->GetSerial() == serial;
	});

	return it != _devices.end() ? *it : nullptr;
}
} // End frox