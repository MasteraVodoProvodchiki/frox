#pragma once

#include "RealSenseModule.h"
#include "RealSenseDevice.h"
#include "RealSenseInspector.h"
#include "RealSenseDeviceManager.h"

namespace frox {

class RealSenseModule : public IRealSenseModule
{
public:
	RealSenseModule();

	// IRealSenseModule overrides
	virtual void InitialiseModule() override;
	virtual void ShutdownModule() override;
	virtual rs2_context* GetHandle() const override
	{
		return _rsContext;
	}

private:
	void InitContext();
	void ReleaseContext();

private:
	rs2_context* _rsContext;
	RealSenseDeviceManagerPtr _deviceManager;
};

} // End frox.