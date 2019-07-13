#pragma once

#include "Shared.h"

namespace frox {

class IDepthSensorModule
{
public:
	FROXDS_API static IDepthSensorModule& Get();
};

} // End frox.
