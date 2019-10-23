#pragma once

#include <SensorInspector.h>

#include "AstraDevice.h"
#include "AstraNative.h"
#include "AstraTypes.h"

namespace frox {

using AstraInspectorPtr = std::shared_ptr<class AstraInspector>;
class AstraInspector : public SensorInspector
{
public:
	AstraInspector(AstraDevicePtr device, EInspectorType type);
	virtual ~AstraInspector();

	static AstraInspectorPtr Create(AstraDevicePtr device, EInspectorType type)
	{
		return std::make_shared<AstraInspector>(device, type);
	}

	// FDepthSensor overrides
	virtual bool Start();
	virtual void Stop();

	virtual bool IsValid() const override;
	virtual ComputeFramePtr ReadFrame() const override;

private:
	int32_t FindProfile(EAstraStreamType streamType, EAstraFormatType format, AstraStreamMode mode, AstraStreamProfile& out) const;
	void EnsureProfileSupported(EAstraStreamType streamType, EAstraFormatType format, AstraStreamMode mode) const;

	bool WaitReadFrame() const;
	void ProcessFrameset(void* frameset) const;
	void ReadDepth(void* frameset) const;
	void ReadColor(void* frameset) const;
	void ReadInfrared(void* frameset) const;

protected:
	AstraDevicePtr _device; // use weak_ptr
	EInspectorType _type;

	ComputeFramePtr _frame;

private:
	AstraStreamProfile _currenProfile;
	bool _bStartedFlag;
};

} // End frox.
