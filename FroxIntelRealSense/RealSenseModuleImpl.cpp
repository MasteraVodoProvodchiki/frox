#include "RealSenseModuleImpl.h"
#include "RealSenseNative.h"

#include <Log.h>

namespace frox {

RealSenseModule::RealSenseModule()
	: _rsContext(nullptr)
{}

void RealSenseModule::InitialiseModule()
{
	InitContext();
	QueryDevices();
}

void RealSenseModule::ShutdownModule()
{
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
	_inspectors.clear();
	_devices.clear();

	if (_rsContext)
	{
		rs2_delete_context(_rsContext);
		_rsContext = nullptr;
	}
}

RealSenseDevicePtr RealSenseModule::NewDevice(rs2_device* handle, const char* name)
{
	auto device = RealSenseDevice::Create(handle);
	return device;
}

SensorInspectorPtr RealSenseModule::NewInspector(RealSenseDevicePtr device, const char* name)
{
	auto inpsector = RealSenseInspector::Create(device);
	return inpsector;
}


void RealSenseModule::QueryDevices()
{
	_devices.clear();
	_inspectors.clear();

	rs2::error_ref e;
	rs2_device_list* list = rs2_query_devices(RealSenseModule::Get().GetHandle(), &e);
	if (list != nullptr)
	{
		const int count = rs2_get_device_count(list, &e);
		for (int i = 0; i < count; i++)
		{
			rs2_device* rsDevice = rs2_create_device(list, i, &e);
			if (rsDevice)
			{
				_devices.push_back(NewDevice(rsDevice, "RealSenseDevice"));
			}

			if (!e.success())
			{
				Log::Error("rs2_create_device failed: {0}", e.get_message());
			}
		}

		rs2_delete_device_list(list);
	}

	for (auto device : _devices)
	{
		device->QueryData();
	}

	for (auto device : _devices)
	{
		_inspectors.push_back(NewInspector(device, "RealSenseInspector"));
	}
}

RealSenseDevicePtr RealSenseModule::GetDeviceById(uint32_t id) const
{
	return id < _devices.size() ? _devices[id] : nullptr;
}

RealSenseDevicePtr RealSenseModule::FindDeviceBySerial(const std::string& serial) const
{
	auto it = std::find_if(_devices.begin(), _devices.end(), [serial](RealSenseDevicePtr device) {
		return device->GetSerial() == serial;
	});

	return it != _devices.end() ? *it : nullptr;
}



} // End frox.