#pragma once

#include "Shared.h"

struct rs2_context;

namespace frox {

class IRealSenseModule
{
public:
	FROXIRS_API static IRealSenseModule& Get();

	virtual void InitialiseModule() = 0;
	virtual void ShutdownModule() = 0;
	virtual rs2_context* GetHandle() const = 0;
};

} // End frox.
