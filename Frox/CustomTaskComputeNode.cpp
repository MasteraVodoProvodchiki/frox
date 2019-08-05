#include "CustomTaskComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(CustomTaskComputeNode)

CustomTaskComputeNode::CustomTaskComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

CustomTaskComputeNode::~CustomTaskComputeNode()
{
}

void CustomTaskComputeNode::AllocateDefaultPins()
{

}

bool CustomTaskComputeNode::IsValid() const
{
	return true;
}

ComputeTask* CustomTaskComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	ComputeNodeImplPtr self = getptr();
	return ComputeTaskHelper::Make([this, self, inputData, outputData]() {
		this->ReceiveExecute(inputData, outputData);
	});
}

bool CustomTaskComputeNode::ReceiveExecute(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	return true;
}

} // End frox