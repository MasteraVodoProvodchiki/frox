#include "DepthSensorModule.h"
#include "DepthSensorModuleImpl.h"

namespace frox {

IDepthSensorModule& IDepthSensorModule::Get()
{
	static DepthSensorModule depthSensorModule;
	return depthSensorModule;
}

} // End frox.