#pragma once

#include "SensorInspector.h"

namespace frox {

class MOCSensorInspector : public SensorInspector
{
public:
	MOCSensorInspector();
	virtual ~MOCSensorInspector();

	// FDepthSensor overrides
	virtual bool Start();
	virtual void Stop();

	virtual bool IsValid() const override;
	virtual ComputeFramePtr ReadFrame() const override;

protected:
	ComputeFramePtr _frame;
};

} // End frox.
