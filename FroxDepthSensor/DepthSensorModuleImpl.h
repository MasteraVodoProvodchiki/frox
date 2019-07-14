#pragma once

#include "DepthSensorModule.h"

#include <vector>

namespace frox {

class DepthSensorModule : public IDepthSensorModule
{
public:
	DepthSensorModule();

	// IDepthSensorModule overrides
	virtual void RegisterDeviceManager(SensorDeviceManager* deviceManager) override;
	virtual void UnRegisterDeviceManager(SensorDeviceManager* deviceManager) override;
	virtual uint32_t GetNumDevices() const override;
	virtual class SensorDevice* FindDevice(const char* serial) const override;
	virtual class SensorDevice* GetDefaultDevice() const override;

private:
	std::vector<SensorDeviceManager*> _managers;
};

} // End frox.
