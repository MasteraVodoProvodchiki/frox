#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class FlipComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(FlipComputeNode, "flip")
	using Super = ComputeNodeImpl;

public:
	FlipComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~FlipComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetVertical(bool vertical);
	FROX_API void SetHorizontal(bool horizontal);

private:
	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<bool> _vertical;
	TExpressionInput<bool> _horizontal;
	TOutputFrame<ComputeFramePtr> _output;
};

} // End frox