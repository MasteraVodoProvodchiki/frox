#include "AstraModule.h"
#include "AstraModuleImpl.h"

namespace frox {

IAstraModule& IAstraModule::Get()
{
	static AstraModule astraModule;
	return astraModule;
}

} // End frox.