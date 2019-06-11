#pragma once

#include "Common.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;

struct ComputeNodeInitializer
{
	const char* Name = nullptr;
};

class FROX_API ComputeNode
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

	void SetInputScalar(uint32_t inId, uint8_t value);
	void SetInputScalar(uint32_t inId, uint16_t value);
	void SetInputScalar(uint32_t inId, uint32_t value);
	void SetInputScalar(uint32_t inId, float value);
};

} // End frox