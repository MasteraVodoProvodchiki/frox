#pragma once

#include "SensorDeviceManager.h"
#include "MOCSensorDevice.h"

#include <vector>

namespace frox {

using MOCSensorDeviceManagerPtr = std::shared_ptr<class MOCSensorDeviceManager>;
class MOCSensorDeviceManager : public SensorDeviceManager
{
public:
	FROXDS_API MOCSensorDeviceManager();
	FROXDS_API virtual ~MOCSensorDeviceManager();

	static MOCSensorDeviceManagerPtr Create()
	{
		return std::make_shared<MOCSensorDeviceManager>();
	}

	// SensorDeviceManager overrides
	virtual uint32_t GetNumDevices() const override;
	virtual SensorDevice* FindDevice(const char* serial) const override;
	virtual class SensorDevice* GetDefaultDevice() const override;

	FROXDS_API MOCSensorDevicePtr CreateDevice();
	FROXDS_API void ClearDevices();

private:
	std::vector<MOCSensorDevicePtr> _devices;
};

} // End frox