#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class CropComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(CropComputeNode, "crop")
	using Super = ComputeNodeImpl;

public:
	CropComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~CropComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetRect(Rect rect);

private:
	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<Rect> _rect;
	TOutputFrame<ComputeFramePtr> _output;
};

} // End frox