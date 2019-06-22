#include "SubFlowComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"
#include "BasicComputeFlow.h"


#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(SubFlowComputeNode)

SubFlowComputeNode::SubFlowComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

SubFlowComputeNode::~SubFlowComputeNode()
{
	if (_subFlow != nullptr)
	{
		_subFlow->Release();
		_subFlow = nullptr;
	}
}

void SubFlowComputeNode::AllocateDefaultPins()
{
	inPinIds.clear();
	outPinIds.clear();

	if (_computeFlowImpl != nullptr)
	{
		// Entries
		const ComputeFlowEntry* entries;
		uint32_t nbEntries = _computeFlowImpl->GetEntries(&entries);
		for (uint32_t index=0; index < nbEntries; ++index)
		{
			const ComputeFlowEntry& entry = entries[index];
			uint32_t inPinId = CreateInput(entry.Name.data());
			inPinIds.push_back(inPinId);
		}

		// Outputs
		const ComputeFlowOutput* outputs;
		uint32_t nbOutputs = _computeFlowImpl->GetOutputs(&outputs);
		for (uint32_t index = 0; index < nbOutputs; ++index)
		{
			const ComputeFlowOutput& output = outputs[index];
			uint32_t outPinId = CreateOutput(output.Name.data());
			outPinIds.push_back(outPinId);
		}
	}
}

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

void SubFlowComputeNode::SetSubFlow(ComputeFlow* subFlow)
{
	if (_subFlow != nullptr)
	{
		_subFlow->Release();
		_subFlow = nullptr;
		_computeFlowImpl = nullptr;
	}

	_subFlow = subFlow;

	if (_subFlow != nullptr)
	{
		BasicComputeFlow* basicComputeFlow = reinterpret_cast<BasicComputeFlow*>(_subFlow);
		_computeFlowImpl = basicComputeFlow->GetComputeFlow();
	}
	
	ReallocatePins();
}

} // End frox