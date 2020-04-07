#pragma once

#include <SensorInspector.h>

#include "Kinect2Device.h"
#include "Native.h"
#include "Types.h"

namespace frox {

using Kinect2InspectorPtr = std::shared_ptr<class Kinect2Inspector>;
class Kinect2Inspector : public SensorInspector
{
public:
	Kinect2Inspector(Kinect2DevicePtr device, EInspectorType type);
	virtual ~Kinect2Inspector();

	static Kinect2InspectorPtr Create(Kinect2DevicePtr device, EInspectorType type)
	{
		return std::make_shared<Kinect2Inspector>(device, type);
	}

	// FDepthSensor overrides
	virtual bool Start();
	virtual void Stop();

	virtual bool IsValid() const override;
	virtual ComputeFramePtr ReadFrame() const override;

private:
	int32_t FindProfile(EKinect2StreamType streamType, EKinect2FormatType format, Kinect2StreamMode mode, Kinect2StreamProfile& out) const;
	void EnsureProfileSupported(EKinect2StreamType streamType, EKinect2FormatType format, Kinect2StreamMode mode) const;

	bool InitDepth();
	bool InitColor();
	bool InitInfrared();

	bool WaitReadFrame() const;
	void ProcessFrameset(ComputeFramePtr frame) const;
	void ReadDepth(ComputeFramePtr frame) const;
	void ReadColor(ComputeFramePtr frame) const;
	void ReadInfrared(ComputeFramePtr frame) const;

protected:
	Kinect2DevicePtr _device; // use weak_ptr
	EInspectorType _type;

	ComputeFramePtr _frame;

private:
	Kinect2StreamProfile _currenProfile;
	bool _bStartedFlag;
};

} // End frox.
