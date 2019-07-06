#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ComputeFlow.h"
#include "ComputeFlowImpl.h"

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
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetSubFlow(ComputeFlow* subFlow);

protected:
	void ClearPins();

private:
	ComputeFlow* _subFlow;
	ComputeFlowImplPtr _computeFlowImpl;

	std::vector<ExpressionInput*> _inPins;
	std::vector<OutputPin*> _outPins;
};


} // End frox