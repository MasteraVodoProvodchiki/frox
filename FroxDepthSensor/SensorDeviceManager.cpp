#include "SensorDeviceManager.h"
#include "DepthSensorModule.h"

namespace frox {

SensorDeviceManager::SensorDeviceManager()
{}

SensorDeviceManager::~SensorDeviceManager()
{
	IDepthSensorModule::Get().UnRegisterDeviceManager(this);
}

} // End frox