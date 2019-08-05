#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class CustomTaskComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(CustomTaskComputeNode, "customtask")
	using Super = ComputeNodeImpl;

public:
	FROX_API CustomTaskComputeNode(const ComputeNodeInitializer& initializer);
	FROX_API virtual ~CustomTaskComputeNode() override;

	// ComputeNode overrides
	FROX_API virtual void AllocateDefaultPins() override;
	FROX_API virtual bool IsValid() const override;
	FROX_API virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;
	
protected:
	FROX_API virtual bool ReceiveExecute(FlowDataImplPtr inputData, FlowDataImplPtr outputData);
};

} // End frox