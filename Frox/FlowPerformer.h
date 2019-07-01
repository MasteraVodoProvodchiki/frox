#pragma once

#include "Common.h"
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
	virtual void Perform(ComputeFlow* flow /* inputData */) = 0;
	virtual void Fetch(ComputeFlow* flow /* outputData */) = 0;
	virtual uint32_t GetNumActiveTasks() const = 0;

	void Release();
};

} // End frox