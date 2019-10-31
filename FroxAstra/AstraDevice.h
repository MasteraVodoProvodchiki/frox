#pragma once

#include <SensorDevice.h>
#include "AstraSensor.h"

namespace frox {

using MultiFrameListenerPtr = std::shared_ptr<class MultiFrameListener>;
using AstraDevicePtr = std::shared_ptr<class AstraDevice>;
class AstraDevice
	: public SensorDevice
	, public std::enable_shared_from_this<AstraDevice>
{
	friend MultiFrameListener;

public:
	AstraDevice(astra::StreamSet streamSet);
	virtual ~AstraDevice();

	AstraDevicePtr GetPtr()
	{
		return shared_from_this();
	}

	static AstraDevicePtr Create(astra::StreamSet streamSet)
	{
		return std::make_shared<AstraDevice>(streamSet);
	}

	static AstraDevicePtr Create(const char* deviceURI)
	{
		return std::make_shared<AstraDevice>(astra::StreamSet(deviceURI));
	}
	
	// SensorDevice overrides
	virtual const char* GetName() const override { return _name.data(); }
	virtual const char* GetSerial() const override { return _serial.data(); }
	virtual SensorInspectorPtr CreateInpector(EInspectorType type) override;

	void QueryData();
	bool FindProfile(AstraStreamProfile profile, AstraStreamProfile& out) const;
	bool SupportsProfile(AstraStreamProfile profile) const;

	void StartStream(EAstraStreamType streamType);
	void StopStream(EAstraStreamType streamType);
	ComputeFramePtr WaitReadFrame(EAstraStreamType streamType);

	astra::StreamReader& GetReader() { return _reader; }

private:
	AstraSensorPtr NewSensor(void* handle, const char* name);
	void InitFrames();

	void StartDepthStream();
	void StartColorStream();
	void StartInfraredStream();

	void StopDepthStream();
	void StopColorStream();
	void StopInfraredStream();

	void UpdateDepth(astra::Frame& frame);
	void UpdateColor(astra::Frame& frame);
	void UpdateInfrared(astra::Frame& frame);

protected:
	std::string _name;
	std::string _serial;

private:
	std::vector<AstraSensorPtr> _sensors;

	astra::StreamSet _streamSet;
	astra::StreamReader _reader;
	MultiFrameListenerPtr _listerner;

	uint32_t _depthStart = 0;
	uint32_t _colorStart = 0;
	uint32_t _infraredStart = 0;

	ComputeFramePtr _depthFrame;
	ComputeFramePtr _colorFrame;
	ComputeFramePtr _infraredFrame;
};

} // End frox.
