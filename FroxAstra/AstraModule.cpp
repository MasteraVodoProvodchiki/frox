#include "AstraModule.h"
#include "AstraModuleImpl.h"

namespace frox {

IAstraModule& IAstraModule::Get()
{
	static AstraModule realSenseModule;
	return realSenseModule;
}

} // End frox.