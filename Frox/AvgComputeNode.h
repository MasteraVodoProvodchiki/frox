#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class AvgComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(AvgComputeNode, "avg")
	using Super = ComputeNodeImpl;

public:
	AvgComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~AvgComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask() override;

protected:
	// ComputeNodeImpl overrides
	virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame) override;

private:
	uint32_t _input;
	uint32_t _output;
};


} // End frox