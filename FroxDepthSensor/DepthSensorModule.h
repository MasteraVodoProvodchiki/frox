#pragma once

#include "Shared.h"

namespace frox {

class IDepthSensorModule
{
public:
	FROXDS_API static IDepthSensorModule& Get();

	virtual void RegisterDeviceManager(class SensorDeviceManager* deviceManager) = 0;
	virtual void UnRegisterDeviceManager(class SensorDeviceManager* deviceManager) = 0;
	virtual uint32_t GetNumDevices() const = 0;
	virtual class SensorDevice* FindDevice(const char* serial) const = 0;
	virtual class SensorDevice* GetDefaultDevice() const = 0;

	template<typename SensorDeviceManagerT>
	std::shared_ptr<SensorDeviceManagerT> CreateDeviceManager()
	{
		auto device = SensorDeviceManagerT::Create();
		this->RegisterDeviceManager(device.get());
		return device;
	}
};

} // End frox.
