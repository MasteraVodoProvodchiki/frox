#pragma once

#include "SensorInspector.h"
#include "MockSensorDevice.h"

namespace frox {

using MockSensorInspectorPtr = std::shared_ptr<class MockSensorInspector>;
class MockSensorInspector : public SensorInspector
{
public:
	MockSensorInspector(MockSensorDevicePtr device, EInspectorType type);
	virtual ~MockSensorInspector();

	static MockSensorInspectorPtr Create(MockSensorDevicePtr device, EInspectorType type)
	{
		return std::make_shared<MockSensorInspector>(device, type);
	}

	virtual bool Start();
	virtual void Stop();

	// SensorInspector overrides
	virtual bool IsValid() const override;
	virtual ComputeFramePtr ReadFrame() const override;

private:
	ComputeFramePtr GenerateFrame() const;

protected:
	MockSensorDevicePtr _device;
	EInspectorType _type;
	mutable ComputeFramePtr _frame;
};

} // End frox.
