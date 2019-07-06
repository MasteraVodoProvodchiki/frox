#include "FrameInfoComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(FrameSizeComputeNode)

FrameSizeComputeNode::FrameSizeComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _output("output")
{}

FrameSizeComputeNode::~FrameSizeComputeNode()
{}

void FrameSizeComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
}

ComputeTask* FrameSizeComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	auto input = _input.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	return ComputeTaskHelper::UnPackProps(input)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input) {
				return input != nullptr && input->IsValid();
			},
			[output](ComputeFramePtr input) {
				Size size = input->GetSize();
				output.SetValue(size);
			}
		);
}

} // End frox