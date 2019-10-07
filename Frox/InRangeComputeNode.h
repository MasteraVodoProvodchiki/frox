#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class InRangeComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(InRangeComputeNode, "inrange")
	using Super = ComputeNodeImpl;

public:
	InRangeComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~InRangeComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetLow(double alpha);
	FROX_API void SetHigh(double beta);

private:
	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<double> _low;
	TExpressionInput<double> _high;
	TOutputFrame<ComputeFramePtr> _output;
};

} // End frox