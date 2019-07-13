#include "RealSenseModule.h"
#include "RealSenseModuleImpl.h"

namespace frox {

IRealSenseModule& IRealSenseModule::Get()
{
	static RealSenseModule realSenseModule;
	return realSenseModule;
}

} // End frox.