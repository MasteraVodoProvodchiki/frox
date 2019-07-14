#pragma once

#include <SensorDevice.h>
#include "RealSenseSensor.h"

namespace frox {

using RealSenseDevicePtr = std::shared_ptr<class RealSenseDevice>;
class RealSenseDevice
	: public SensorDevice
	, public std::enable_shared_from_this<RealSenseDevice>
{
public:
	RealSenseDevice(struct rs2_device* rsDevice);
	virtual ~RealSenseDevice();

	RealSenseDevicePtr GetPtr()
	{
		return shared_from_this();
	}

	static RealSenseDevicePtr Create(struct rs2_device* rsDevice)
	{
		return std::make_shared<RealSenseDevice>(rsDevice);
	}

	// SensorDevice overrides
	virtual const char* GetName() const override { return _name.data(); }
	virtual const char* GetSerial() const override { return _serial.data(); }
	virtual SensorInspectorPtr CreateInpector(EInspectorType type) override;

	void QueryData();
	bool FindProfile(RealSenseStreamProfile profile, RealSenseStreamProfile& out) const;
	bool SupportsProfile(RealSenseStreamProfile profile) const;

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
