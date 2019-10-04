#pragma once

#include "SensorDeviceManager.h"
#include "MockSensorDevice.h"

#include <vector>

namespace frox {

using MockSensorDeviceManagerPtr = std::shared_ptr<class MockSensorDeviceManager>;
class MockSensorDeviceManager : public SensorDeviceManager
{
public:
	FROXDS_API MockSensorDeviceManager();
	FROXDS_API virtual ~MockSensorDeviceManager();

	static MockSensorDeviceManagerPtr Create()
	{
		return std::make_shared<MockSensorDeviceManager>();
	}

	// SensorDeviceManager overrides
	virtual uint32_t GetNumDevices() const override;
	virtual SensorDevice* FindDevice(const char* serial) const override;
	virtual class SensorDevice* GetDefaultDevice() const override;

	FROXDS_API MockSensorDevicePtr CreateDevice();
	FROXDS_API void ClearDevices();

private:
	std::vector<MockSensorDevicePtr> _devices;
};

} // End frox