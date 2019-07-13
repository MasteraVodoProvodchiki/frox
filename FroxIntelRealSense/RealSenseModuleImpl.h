#pragma once

#include "RealSenseModule.h"
#include "RealSenseDevice.h"
#include "RealSenseInspector.h"

struct rs2_context;

namespace frox {

class RealSenseModule : public IRealSenseModule
{
public:
	RealSenseModule();

	virtual void InitialiseModule();
	virtual void ShutdownModule();

	void InitContext();
	void ReleaseContext();

	rs2_context* GetHandle() const
	{
		return _rsContext;
	}

private:
	RealSenseDevicePtr NewDevice(rs2_device* Handle, const char* Name);
	SensorInspectorPtr NewInspector(RealSenseDevicePtr device, const char* Name);

	void QueryDevices();
	RealSenseDevicePtr GetDeviceById(uint32_t id) const;
	RealSenseDevicePtr FindDeviceBySerial(const std::string& serial) const;


private:
	rs2_context* _rsContext;
	std::vector<RealSenseDevicePtr> _devices;
	std::vector<SensorInspectorPtr> _inspectors;

};

} // End frox.
