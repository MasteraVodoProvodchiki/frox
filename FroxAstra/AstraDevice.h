#pragma once

#include <SensorDevice.h>
#include "AstraSensor.h"

namespace frox {

using AstraDevicePtr = std::shared_ptr<class AstraDevice>;
class AstraDevice
	: public SensorDevice
	, public std::enable_shared_from_this<AstraDevice>
{
public:
	AstraDevice(void* rsDevice);
	virtual ~AstraDevice();

	AstraDevicePtr GetPtr()
	{
		return shared_from_this();
	}

	static AstraDevicePtr Create(void* hDevice)
	{
		return std::make_shared<AstraDevice>(hDevice);
	}

	// SensorDevice overrides
	virtual const char* GetName() const override { return _name.data(); }
	virtual const char* GetSerial() const override { return _serial.data(); }
	virtual SensorInspectorPtr CreateInpector(EInspectorType type) override;

	void QueryData();
	bool FindProfile(AstraStreamProfile profile, AstraStreamProfile& out) const;
	bool SupportsProfile(AstraStreamProfile profile) const;

private:
	AstraSensorPtr NewSensor(void* handle, const char* name);

protected:
	std::string _name;
	std::string _serial;

private:
	std::vector<AstraSensorPtr> _sensors;
	void* _hDevice;
};

} // End frox.
