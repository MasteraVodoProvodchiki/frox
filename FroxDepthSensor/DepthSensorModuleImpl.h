#pragma once

#include "DepthSensorModule.h"

struct rs2_context;

namespace frox {

class DepthSensorModule : public IDepthSensorModule
{
public:
	DepthSensorModule();
};

} // End frox.
