#pragma once

#include <SensorDevice.h>
#include "Kinect2Sensor.h"

namespace frox {

// using MultiFrameListenerPtr = std::shared_ptr<class MultiFrameListener>;
using Kinect2DevicePtr = std::shared_ptr<class Kinect2Device>;
class Kinect2Device
	: public SensorDevice
	, public std::enable_shared_from_this<Kinect2Device>
{
public:
	Kinect2Device(IKinectSensor* kinectSensor);
	virtual ~Kinect2Device();

	Kinect2DevicePtr GetPtr()
	{
		return shared_from_this();
	}

	static Kinect2DevicePtr Create(IKinectSensor* kinectSensor)
	{
		return std::make_shared<Kinect2Device>(kinectSensor);
	}

	// SensorDevice overrides
	virtual const char* GetName() const override { return _name.data(); }
	virtual const char* GetSerial() const override { return _serial.data(); }
	virtual SensorInspectorPtr CreateInpector(EInspectorType type) override;

	void QueryData();
	bool FindProfile(Kinect2StreamProfile profile, Kinect2StreamProfile& out) const;
	bool SupportsProfile(Kinect2StreamProfile profile) const;

	void StartStream(EKinect2StreamType streamType);
	void StopStream(EKinect2StreamType streamType);
	ComputeFramePtr WaitReadFrame(EKinect2StreamType streamType);

	IKinectSensor* GetKinectSensor() const { return _kinectSensor; }

private:
	Kinect2SensorPtr NewSensor(void* handle, const char* name);
	void InitFrames();

	void StartDepthStream();
	void StartColorStream();
	void StartInfraredStream();

	void StopDepthStream();
	void StopColorStream();
	void StopInfraredStream();

	void UpdateDepth();
	void UpdateColor();
	void UpdateInfrared();

protected:
	std::string _name;
	std::string _serial;

private:
	std::vector<Kinect2SensorPtr> _sensors;

	// Current Kinect
	IKinectSensor* _kinectSensor;

	// Depth reader
	IDepthFrameReader* _depthFrameReader;
	IColorFrameReader* _colorFrameReader;
	IInfraredFrameReader* _infraredFrameReader;

	// MultiFrameListenerPtr _listerner;

	uint32_t _depthStart = 0;
	uint32_t _colorStart = 0;
	uint32_t _infraredStart = 0;

	ComputeFramePtr _depthFrame;
	ComputeFramePtr _colorFrame;
	ComputeFramePtr _infraredFrame;
};

} // End frox.
