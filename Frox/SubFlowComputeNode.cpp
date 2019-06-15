#include "SubFlowComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(SubFlowComputeNode)

SubFlowComputeNode::SubFlowComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

SubFlowComputeNode::~SubFlowComputeNode()
{}

void SubFlowComputeNode::AllocateDefaultPins()
{}

void SubFlowComputeNode::OnInputChanged(uint32_t inId, ComputeFramePtr frame)
{}

bool SubFlowComputeNode::IsValid() const
{
	return _subFlow != nullptr;
}

ComputeTask* SubFlowComputeNode::CreateComputeTask()
{
	ComputeFlow* subFlow = _subFlow;

	return ComputeTaskUtils::Make([]() {
		// ??
	});
}

} // End frox