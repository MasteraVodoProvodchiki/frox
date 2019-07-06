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
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeFramePtr> _input;
	TOutputFrame<ComputeFramePtr> _output;
};


} // End frox