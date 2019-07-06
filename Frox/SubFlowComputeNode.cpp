#include "SubFlowComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
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
	ClearPins();

	if (_computeFlowImpl != nullptr)
	{
		// Entries
		const ComputeFlowEntry* entries;
		uint32_t nbEntries = _computeFlowImpl->GetEntries(&entries);
		for (uint32_t index=0; index < nbEntries; ++index)
		{
			const ComputeFlowEntry& entry = entries[index];
			auto pin = new ExpressionInput(entry.Name.data());
			_inPins.push_back(pin);	
		}

		// Outputs
		const ComputeFlowOutput* outputs;
		uint32_t nbOutputs = _computeFlowImpl->GetOutputs(&outputs);
		for (uint32_t index = 0; index < nbOutputs; ++index)
		{
			const ComputeFlowOutput& output = outputs[index];
			auto pin = new OutputPin(output.Name.data());
			_outPins.push_back(pin);
		}
	}
}

bool SubFlowComputeNode::IsValid() const
{
	return _subFlow != nullptr;
}

ComputeTask* SubFlowComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	ComputeFlow* subFlow = _subFlow;

	return ComputeTaskHelper::Make([]() {
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

void SubFlowComputeNode::ClearPins()
{
	for (Pin* pin : _inPins)
	{
		delete pin;
	}
	_inPins.clear();

	for (Pin* pin : _outPins)
	{
		delete pin;
	}
	_outPins.clear();
}

} // End frox