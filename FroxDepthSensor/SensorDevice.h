#pragma once

#include "Shared.h"
#include "SensorInspector.h"

namespace frox {

enum class EInspectorType
{
	Depth,
	Color,
	Infrared
};

// using SensorDevicePtr = std::shared_ptr<class SensorDevice>;
class SensorDevice
{
public:
	SensorDevice()
	{}

	virtual ~SensorDevice()
	{}

	virtual const char* GetName() const = 0;
	virtual const char* GetSerial() const = 0;
	virtual SensorInspectorPtr CreateInpector(EInspectorType type) = 0;
	virtual ComputeFramePtr MapDepthFrameToColorFrame(ComputeFramePtr depthFrame, ComputeFramePtr colorFrame) = 0;
};

} // End frox.
