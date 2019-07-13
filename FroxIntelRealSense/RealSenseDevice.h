#pragma once

#include "RealSenseSensor.h"

namespace frox {

using RealSenseDevicePtr = std::shared_ptr<class RealSenseDevice>;
class RealSenseDevice : std::enable_shared_from_this<RealSenseSensor>
{
	RealSenseDevice(struct rs2_device* rsDevice);

public:	
	virtual ~RealSenseDevice();

	static RealSenseDevicePtr Create(struct rs2_device* rsDevice)
	{
		return std::make_shared<RealSenseDevice>(rsDevice);
	}

	void QueryData();
	bool FindProfile(RealSenseStreamProfile profile, RealSenseStreamProfile& out) const;
	bool SupportsProfile(RealSenseStreamProfile profile) const;

	const std::string& GetName() const { return _name; }
	const std::string& GetSerial() const { return _serial; }

private:
	RealSenseSensorPtr NewSensor(struct rs2_sensor* handle, const char* name);

protected:
	std::string _name;
	std::string _serial;

	// Current Kinect
	struct rs2_device* _rsDevice;

private:
	std::vector<RealSenseSensorPtr> _sensors;
};

} // End frox.
