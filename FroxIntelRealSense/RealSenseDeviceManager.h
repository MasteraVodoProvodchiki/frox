#pragma once

#include <SensorDeviceManager.h>
#include "RealSenseDevice.h"
#include "RealSenseInspector.h"

#include <vector>

namespace frox {

using RealSenseDeviceManagerPtr = std::shared_ptr<class RealSenseDeviceManager>;
class RealSenseDeviceManager : public SensorDeviceManager
{
public:
	FROXIRS_API RealSenseDeviceManager();
	FROXIRS_API virtual ~RealSenseDeviceManager();

	static RealSenseDeviceManagerPtr Create()
	{
		return std::make_shared<RealSenseDeviceManager>();
	}

	// SensorDeviceManager overrides
	virtual uint32_t GetNumDevices() const override;
	virtual class SensorDevice* FindDevice(const char* serial) const override;
	virtual class SensorDevice* GetDefaultDevice() const override;

public:
	RealSenseDevicePtr NewDevice(rs2_device* Handle, const char* Name);
	void QueryDevices();
	RealSenseDevicePtr GetDeviceById(uint32_t id) const;
	RealSenseDevicePtr FindDeviceBySerial(const char* serial) const;

private:
	std::vector<RealSenseDevicePtr> _devices;
};

} // End frox