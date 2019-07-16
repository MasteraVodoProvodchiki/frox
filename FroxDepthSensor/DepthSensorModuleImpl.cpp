#include "DepthSensorModuleImpl.h"
#include "SensorDeviceManager.h"

#include <algorithm>

namespace frox {

DepthSensorModule::DepthSensorModule()
{}

void DepthSensorModule::InitialiseModule()
{}

void DepthSensorModule::ShutdownModule()
{}

void DepthSensorModule::RegisterDeviceManager(SensorDeviceManager* deviceManager)
{
	_managers.push_back(deviceManager);
}

void DepthSensorModule::UnRegisterDeviceManager(SensorDeviceManager* deviceManager)
{
	auto it = std::remove(_managers.begin(), _managers.end(), deviceManager);
	_managers.erase(it, _managers.end());
}

uint32_t DepthSensorModule::GetNumDevices() const
{
	uint32_t numDevices = 0;
	for (SensorDeviceManager* manager : _managers)
	{
		numDevices += manager->GetNumDevices();
	}

	return numDevices;
}

SensorDevice* DepthSensorModule::FindDevice(const char* serial) const
{
	for (SensorDeviceManager* manager : _managers)
	{
		SensorDevice* device = manager->FindDevice(serial);
		if (device != nullptr)
		{
			return device;
		}
	}

	return nullptr;
}

SensorDevice* DepthSensorModule::GetDefaultDevice() const
{
	for (SensorDeviceManager* manager : _managers)
	{
		SensorDevice* device = manager->GetDefaultDevice();
		if (device != nullptr)
		{
			return device;
		}
	}

	return nullptr;
}

} // End frox.
