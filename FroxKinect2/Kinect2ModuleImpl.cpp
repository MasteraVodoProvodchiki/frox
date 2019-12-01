#include "Kinect2ModuleImpl.h"
#include "Native.h"


#include <DepthSensorModule.h>

#include <Log.h>

namespace frox {

Kinect2Module::Kinect2Module()
{}

void Kinect2Module::InitialiseModule()
{
	InitContext();

	_deviceManager = IDepthSensorModule::Get().CreateDeviceManager<Kinect2DeviceManager>();
	_deviceManager->QueryDevices();
}

void Kinect2Module::ShutdownModule()
{
	IDepthSensorModule::Get().UnRegisterDeviceManager(_deviceManager.get());
	_deviceManager = nullptr;
	ReleaseContext();
}

void Kinect2Module::InitContext()
{
	// Init kinect2 SDK
}

void Kinect2Module::ReleaseContext()
{
	// Release kinect2 SDK
}

} // End frox.