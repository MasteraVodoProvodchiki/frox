#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ComputeFlow.h"

namespace frox {

class ComputeTask;
class SubFlowComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(SubFlowComputeNode, "subflow")
	using Super = ComputeNodeImpl;

public:
	SubFlowComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~SubFlowComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask() override;

protected:
	// ComputeNodeImpl overrides
	virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame) override;

private:
	ComputeFlow* _subFlow;
};


} // End frox