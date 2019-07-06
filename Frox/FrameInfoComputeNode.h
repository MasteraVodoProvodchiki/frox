#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class FrameSizeComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(FrameSizeComputeNode, "framesize")
	using Super = ComputeNodeImpl;

public:
	FrameSizeComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~FrameSizeComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeFramePtr> _input;
	TOutput<Size> _output;
};


} // End frox