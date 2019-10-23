#pragma once

#include "Shared.h"

struct rs2_context;

namespace frox {

class IAstraModule
{
public:
	FROXASTRA_API static IAstraModule& Get();

	virtual void InitialiseModule() = 0;
	virtual void ShutdownModule() = 0;
	virtual void* GetHandle() const = 0;
};

} // End frox.
