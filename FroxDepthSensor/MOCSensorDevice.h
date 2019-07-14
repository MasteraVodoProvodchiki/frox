#pragma once

#include "SensorDevice.h"

#include <string>

namespace frox {

using MOCSensorDevicePtr = std::shared_ptr<class MOCSensorDevice>;
class MOCSensorDevice
	: public SensorDevice
	, public std::enable_shared_from_this<MOCSensorDevice>
{
public:
	MOCSensorDevice(const char* name, const char* serial);
	virtual ~MOCSensorDevice();

	MOCSensorDevicePtr GetPtr()
	{
		return shared_from_this();
	}

	static MOCSensorDevicePtr Create(const char* name, const char* serial)
	{
		return std::make_shared<MOCSensorDevice>(name, serial);
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