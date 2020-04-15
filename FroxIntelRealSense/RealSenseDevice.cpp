#include "RealSenseDevice.h"
#include "RealSenseInspector.h"
#include <librealsense2/rs_advanced_mode.hpp>

namespace frox {

RealSenseDevice::RealSenseDevice(rs2_device* rsDevice)
	: _rsDevice(rsDevice)
{}

RealSenseDevice::~RealSenseDevice()
{
	_sensors.clear();

	// close the Kinect Sensor
	if (_rsDevice != nullptr)
	{
		rs2_delete_device(_rsDevice);
	}
}

SensorInspectorPtr RealSenseDevice::CreateInpector(EInspectorType type)
{
	auto inspector = RealSenseInspector::Create(this->GetPtr(), type);
	inspector->Start();
	return inspector;
}

bool RealSenseDevice::MapDepthFrameToColorFrame(ComputeFramePtr depthFrame, ComputeFramePtr colorFrame, ComputeFramePtr output)
{
	//TODO: implement mapping
	return false;
}

void RealSenseDevice::QueryData()
{
	_sensors.clear();

	rs2::error_ref e;
	_name = rs2_get_device_info(_rsDevice, RS2_CAMERA_INFO_NAME, &e);
	_serial = rs2_get_device_info(_rsDevice, RS2_CAMERA_INFO_SERIAL_NUMBER, &e);

	rs2_sensor_list* list = rs2_query_sensors(_rsDevice, &e);
	if (list != nullptr)
	{
		const int count = rs2_get_sensors_count(list, &e);
		for (int i = 0; i < count; i++)
		{
			rs2_sensor* rsSensor = rs2_create_sensor(list, i, &e);
			if (rsSensor != nullptr)
			{
				_sensors.push_back(NewSensor(rsSensor, "RealSenseSensor"));
			}
		}

		rs2_delete_sensor_list(list);
	}

	for (auto sensor : _sensors)
	{
		sensor->QueryData();
	}
}

bool RealSenseDevice::FindProfile(RealSenseStreamProfile profile, RealSenseStreamProfile& out) const
{
	for (auto sensor : _sensors)
	{
		if (sensor->FindProfile(profile, out))
		{
			return true;
		}
	}

	return false;
}

bool RealSenseDevice::SupportsProfile(RealSenseStreamProfile Profile) const
{
	for (auto sensor : _sensors)
	{
		if (sensor->SupportsProfile(Profile))
		{
			return true;
		}
	}

	return false;
}

RealSenseSensorPtr RealSenseDevice::NewSensor(rs2_sensor* handle, const char* name)
{
	RealSenseSensorPtr sensor = RealSenseSensor::Create(handle);
	return sensor;
}

} // End frox.
