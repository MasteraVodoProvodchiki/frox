#include "RealSenseDeviceManager.h"
#include "RealSenseModule.h"

#include <Log.h>

namespace frox {

RealSenseDeviceManager::RealSenseDeviceManager()
{}

RealSenseDeviceManager::~RealSenseDeviceManager()
{}

uint32_t RealSenseDeviceManager::GetNumDevices() const
{
	return _devices.size();
}

SensorDevice* RealSenseDeviceManager::FindDevice(const char* serial) const
{
	RealSenseDevicePtr device = FindDeviceBySerial(serial);
	return device.get();
}

SensorDevice* RealSenseDeviceManager::GetDefaultDevice() const
{
	if (!_devices.empty())
	{
		return _devices.front().get();
	}

	return nullptr;
}

RealSenseDevicePtr RealSenseDeviceManager::NewDevice(rs2_device* handle, const char* name)
{
	auto device = RealSenseDevice::Create(handle);
	return device;
}

void RealSenseDeviceManager::QueryDevices()
{
	_devices.clear();

	rs2::error_ref e;
	rs2_device_list* list = rs2_query_devices(IRealSenseModule::Get().GetHandle(), &e);
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
}

RealSenseDevicePtr RealSenseDeviceManager::GetDeviceById(uint32_t id) const
{
	return id < _devices.size() ? _devices[id] : nullptr;
}

RealSenseDevicePtr RealSenseDeviceManager::FindDeviceBySerial(const char* serial) const
{
	auto it = std::find_if(_devices.begin(), _devices.end(), [serial](RealSenseDevicePtr device) {
		return device->GetSerial() == serial;
	});

	return it != _devices.end() ? *it : nullptr;
}
} // End frox