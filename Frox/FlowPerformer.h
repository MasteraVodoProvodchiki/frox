#pragma once

#include "Common.h"
#include "DebugFlowData.h"

#include <memory>

namespace frox {

class IComputeFlowListerner
{
public:
	virtual void OnPerformed() = 0;
};

class FROX_API FlowPerformer
{
public:
	virtual ~FlowPerformer() {}
	virtual void Perform(ComputeFlow* flow, FlowData* inputData) = 0;
	virtual void Fetch(ComputeFlow* flow, FlowData* outputData) = 0;
	virtual uint32_t GetNumActiveTasks() const = 0;

	void Release();

	// Access to performer data for debug purposes only.
	// Trying to use it without proper preparations can and will cause various errors.
	// Works only if library was buit with FROX_DEBUG flag.
	virtual DebugFlowDataPtr GetLatestPerformedDataForDebug() const = 0;
};

} // End frox