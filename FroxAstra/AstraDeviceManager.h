#pragma once

#include <SensorDeviceManager.h>
#include "AstraDevice.h"
#include "AstraInspector.h"

#include <vector>

namespace frox {

using AstraDeviceManagerPtr = std::shared_ptr<class AstraDeviceManager>;
class AstraDeviceManager : public SensorDeviceManager
{
public:
	FROXASTRA_API AstraDeviceManager();
	FROXASTRA_API virtual ~AstraDeviceManager();

	static AstraDeviceManagerPtr Create()
	{
		return std::make_shared<AstraDeviceManager>();
	}

	// SensorDeviceManager overrides
	virtual uint32_t GetNumDevices() const override;
	virtual class SensorDevice* FindDevice(const char* serial) const override;
	virtual class SensorDevice* GetDefaultDevice() const override;

public:
	AstraDevicePtr NewDevice(void* handle, const char* name);
	void QueryDevices();
	AstraDevicePtr GetDeviceById(uint32_t id) const;
	AstraDevicePtr FindDeviceBySerial(const char* serial) const;

private:
	std::vector<AstraDevicePtr> _devices;
};

} // End frox