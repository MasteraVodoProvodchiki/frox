#pragma once

#include "Shared.h"

namespace frox {

class IKinect2Module
{
public:
	FROXASTRA_API static IKinect2Module& Get();

	virtual void InitialiseModule() = 0;
	virtual void ShutdownModule() = 0;
	virtual void* GetHandle() const = 0;
};

} // End frox.
