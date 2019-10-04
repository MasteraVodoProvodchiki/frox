#pragma once

#include "SensorDevice.h"

#include <string>

namespace frox {

using MockSensorDevicePtr = std::shared_ptr<class MockSensorDevice>;
class MockSensorDevice
	: public SensorDevice
	, public std::enable_shared_from_this<MockSensorDevice>
{
public:
	MockSensorDevice(const char* name, const char* serial);
	virtual ~MockSensorDevice();

	MockSensorDevicePtr GetPtr()
	{
		return shared_from_this();
	}

	static MockSensorDevicePtr Create(const char* name, const char* serial)
	{
		return std::make_shared<MockSensorDevice>(name, serial);
	}

	// SensorDevice overrides
	virtual const char* GetName() const override;
	virtual const char* GetSerial() const override;
	virtual SensorInspectorPtr CreateInpector(EInspectorType type) override;

private:
	std::string _name;
	std::string _serial;
};

} // End frox