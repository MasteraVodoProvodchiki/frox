#pragma once

#include "Kinect2Module.h"
#include "Kinect2Device.h"
#include "Kinect2Inspector.h"
#include "Kinect2DeviceManager.h"

namespace frox {

class Kinect2Module : public IKinect2Module
{
public:
	Kinect2Module();

	// IRealSenseModule overrides
	virtual void InitialiseModule() override;
	virtual void ShutdownModule() override;
	virtual void* GetHandle() const override
	{
		return nullptr;
	}

private:
	void InitContext();
	void ReleaseContext();

private:
	Kinect2DeviceManagerPtr _deviceManager;
};

} // End frox.