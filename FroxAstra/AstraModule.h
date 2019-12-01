#pragma once

#include "Shared.h"

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
