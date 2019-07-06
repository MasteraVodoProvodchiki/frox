#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class ConvertToComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(ConvertToComputeNode, "convertto")
	using Super = ComputeNodeImpl;

public:
	ConvertToComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~ConvertToComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetType(EComputeFrameType type);
	FROX_API void SetAlpha(double alpha);
	FROX_API void SetBeta(double beta);

private:
	TExpressionInput<ComputeFramePtr> _input;
	EComputeFrameType _type;
	TExpressionInput<double> _alpha;
	TExpressionInput<double> _beta;
	TOutputFrame<ComputeFramePtr> _output;
};

} // End frox