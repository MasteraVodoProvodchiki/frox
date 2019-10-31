#include "AstraModuleImpl.h"
#include "AstraNative.h"


#include <DepthSensorModule.h>

#include <Log.h>

namespace frox {

AstraModule::AstraModule()
{}

void AstraModule::InitialiseModule()
{
	InitContext();

	_deviceManager = IDepthSensorModule::Get().CreateDeviceManager<AstraDeviceManager>();
	_deviceManager->QueryDevices();
}

void AstraModule::ShutdownModule()
{
	IDepthSensorModule::Get().UnRegisterDeviceManager(_deviceManager.get());
	_deviceManager = nullptr;
	ReleaseContext();
}

void AstraModule::InitContext()
{
	auto status = astra::initialize();
	if (status != ASTRA_STATUS_SUCCESS)
	{
		Log::Error("unable to initialize ", "AstraModule");
		return;
	}

}

void AstraModule::ReleaseContext()
{
	auto status = astra::terminate();
	if (status != ASTRA_STATUS_SUCCESS)
	{
		Log::Error("unable to terminate ", "AstraModule");
		return;
	}
}

} // End frox.