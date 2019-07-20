#pragma once

#include <ComputeFrame.h>

namespace frox {

using SensorInspectorPtr = std::shared_ptr<class SensorInspector>;
class SensorInspector : public std::enable_shared_from_this<SensorInspector>
{
public:
	virtual ~SensorInspector()
	{}

	virtual bool IsValid() const = 0;
	virtual ComputeFramePtr ReadFrame() const = 0;	
};

} // End frox