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
	virtual bool MapDepthFrameToColorFrame(ComputeFramePtr depthFrame, ComputeFramePtr colorFrame, ComputeFramePtr output) override;

	FROXDS_API void SetDepthFrame(ComputeFramePtr frame);
	FROXDS_API void SetColorFrame(ComputeFramePtr frame);
	FROXDS_API void SetInfraredFrame(ComputeFramePtr frame);

	FROXDS_API ComputeFramePtr GetFrame(EInspectorType type) const;

private:
	std::string _name;
	std::string _serial;

	ComputeFramePtr _depthFrame;
	ComputeFramePtr _colorFrame;
	ComputeFramePtr _infraredFrame;
};

} // End frox