#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ExpressionInput.h"

namespace frox {

class ComputeTask;
class ResizeComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(ResizeComputeNode, "resize")
	using Super = ComputeNodeImpl;

public:
	ResizeComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~ResizeComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetSize(Size rect);

protected:
	// ComputeNodeImpl overrides
	// virtual void OnPostInit() override;

private:
	uint32_t _input;
	uint32_t _output;

	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<Size> _size;
};

} // End frox