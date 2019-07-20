#pragma once

#include "SensorInspector.h"
#include "MOCSensorDevice.h"

namespace frox {

using MOCSensorInspectorPtr = std::shared_ptr<class MOCSensorInspector>;
class MOCSensorInspector : public SensorInspector
{
public:
	MOCSensorInspector(MOCSensorDevicePtr device, EInspectorType type);
	virtual ~MOCSensorInspector();

	static MOCSensorInspectorPtr Create(MOCSensorDevicePtr device, EInspectorType type)
	{
		return std::make_shared<MOCSensorInspector>(device, type);
	}

	// FDepthSensor overrides
	virtual bool Start();
	virtual void Stop();

	virtual bool IsValid() const override;
	virtual ComputeFramePtr ReadFrame() const override;

protected:
	MOCSensorDevicePtr _device;
	EInspectorType _type;
	ComputeFramePtr _frame;
};

} // End frox.
