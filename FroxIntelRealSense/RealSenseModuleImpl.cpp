#include "RealSenseModuleImpl.h"
#include "RealSenseNative.h"


#include <DepthSensorModule.h>

#include <Log.h>

namespace frox {

RealSenseModule::RealSenseModule()
	: _rsContext(nullptr)
{}

void RealSenseModule::InitialiseModule()
{
	InitContext();

	_deviceManager = IDepthSensorModule::Get().CreateDeviceManager<RealSenseDeviceManager>();
	_deviceManager->QueryDevices();
}

void RealSenseModule::ShutdownModule()
{
	IDepthSensorModule::Get().UnRegisterDeviceManager(_deviceManager.get());
	_deviceManager = nullptr;
	ReleaseContext();
}

void RealSenseModule::InitContext()
{
	rs2::error_ref e;
	_rsContext = rs2_create_context(RS2_API_VERSION, &e);
	if (!e.success())
	{
		Log::Error("rs2_create_context failed: {0}", e.get_message());
	}
}

void RealSenseModule::ReleaseContext()
{
	if (_rsContext)
	{
		rs2_delete_context(_rsContext);
		_rsContext = nullptr;
	}
}

} // End frox.