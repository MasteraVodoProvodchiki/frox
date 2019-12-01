#include "Kinect2DeviceManager.h"
#include "Kinect2Module.h"

#include <Log.h>
#include <algorithm>

namespace frox {

Kinect2DeviceManager::Kinect2DeviceManager()
{}

Kinect2DeviceManager::~Kinect2DeviceManager()
{}

uint32_t Kinect2DeviceManager::GetNumDevices() const
{
	return _devices.size();
}

SensorDevice* Kinect2DeviceManager::FindDevice(const char* serial) const
{
	Kinect2DevicePtr device = FindDeviceBySerial(serial);
	return device.get();
}

SensorDevice* Kinect2DeviceManager::GetDefaultDevice() const
{
	if (!_devices.empty())
	{
		return _devices.front().get();
	}

	return nullptr;
}

Kinect2DevicePtr Kinect2DeviceManager::NewDevice(IKinectSensor* pKinectSensor, const char* name)
{
	auto device = Kinect2Device::Create(pKinectSensor);
	return device;
}

void Kinect2DeviceManager::QueryDevices()
{
	_devices.clear();

	HRESULT hr;

	// Read devices
	IKinectSensor* pKinectSensor = nullptr;;
	hr = GetDefaultKinectSensor(&pKinectSensor);
	if (SUCCEEDED(hr))
	{
		_devices.push_back(NewDevice(pKinectSensor, "Kinect2Device"));
	}

	for (auto device : _devices)
	{
		device->QueryData();
	}
}

Kinect2DevicePtr Kinect2DeviceManager::GetDeviceById(uint32_t id) const
{
	return id < _devices.size() ? _devices[id] : nullptr;
}

Kinect2DevicePtr Kinect2DeviceManager::FindDeviceBySerial(const char* serial) const
{
	auto it = std::find_if(_devices.begin(), _devices.end(), [serial](Kinect2DevicePtr device) {
		return device->GetSerial() == serial;
	});

	return it != _devices.end() ? *it : nullptr;
}
} // End frox