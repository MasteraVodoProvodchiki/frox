#pragma once

#include "Common.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;

struct ComputeNodeInitializer
{
	const char* Name = nullptr;
};

class ComputeNode
{
public:
	ComputeNode(const ComputeNodeInitializer& initializer)
	{}

	virtual ~ComputeNode() {}

	virtual void AllocateDefaultPins() = 0;
	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) = 0;
	virtual bool IsValid() const = 0;
	virtual ComputeTask* CreateComputeTask() = 0;
};

} // End frox