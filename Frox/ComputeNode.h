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

	virtual const char* GetName() const = 0;
	virtual void AllocateDefaultPins() = 0;
	virtual bool IsValid() const = 0;

	virtual int32_t FindInputByName(const char* pinName) const = 0;
	virtual int32_t FindOutputByName(const char* pinName) const = 0;
};

} // End frox