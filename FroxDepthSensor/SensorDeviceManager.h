#pragma once

#include "Shared.h"

namespace frox {

using SensorDeviceManagerPtr = std::shared_ptr<class SensorDeviceManager>;
class SensorDeviceManager : std::enable_shared_from_this<SensorDeviceManager>
{
public:
	FROXDS_API SensorDeviceManager();
	FROXDS_API virtual ~SensorDeviceManager();

	virtual uint32_t GetNumDevices() const = 0;
	virtual class SensorDevice* FindDevice(const char* serial) const = 0;
	virtual class SensorDevice* GetDefaultDevice() const = 0;
};

} // End frox.
