#pragma once

#include "AstraModule.h"
#include "AstraDevice.h"
#include "AstraInspector.h"
#include "AstraDeviceManager.h"

namespace frox {

class AstraModule : public IAstraModule
{
public:
	AstraModule();

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
	AstraDeviceManagerPtr _deviceManager;
};

} // End frox.