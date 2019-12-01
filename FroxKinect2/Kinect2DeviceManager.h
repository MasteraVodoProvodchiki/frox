#pragma once

#include <SensorDeviceManager.h>
#include "Kinect2Device.h"
#include "Kinect2Inspector.h"

#include <vector>

namespace frox {

using Kinect2DeviceManagerPtr = std::shared_ptr<class Kinect2DeviceManager>;
class Kinect2DeviceManager : public SensorDeviceManager
{
public:
	FROXASTRA_API Kinect2DeviceManager();
	FROXASTRA_API virtual ~Kinect2DeviceManager();

	static Kinect2DeviceManagerPtr Create()
	{
		return std::make_shared<Kinect2DeviceManager>();
	}

	// SensorDeviceManager overrides
	virtual uint32_t GetNumDevices() const override;
	virtual class SensorDevice* FindDevice(const char* serial) const override;
	virtual class SensorDevice* GetDefaultDevice() const override;

public:
	Kinect2DevicePtr NewDevice(IKinectSensor* pKinectSensor, const char* name);
	void QueryDevices();
	Kinect2DevicePtr GetDeviceById(uint32_t id) const;
	Kinect2DevicePtr FindDeviceBySerial(const char* serial) const;
	
private:
	std::vector<Kinect2DevicePtr> _devices;
};

} // End frox