#include "ResizeComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>
#include <algorithm>

namespace frox {

namespace functions {

void Resize(ComputeFramePtr input, ComputeFramePtr output)
{
}

} // End functions

FROX_COMPUTENODE_IMPL(ResizeComputeNode)

ResizeComputeNode::ResizeComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _output("output")
	, _size("size", Size{1, 1})
{}

ResizeComputeNode::~ResizeComputeNode()
{}

void ResizeComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_size);
	RegisterOutput(&_output);
}

bool ResizeComputeNode::IsValid() const
{
	return true;
}

ComputeTask* ResizeComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	auto input = _input.GetValue(inputData);
	auto size = _size.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	return
		ComputeTaskHelper::UnPackProps(input, size)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, Size size) {
				return input != nullptr && input->IsValid() && size.IsValid();
			},
			[output](ComputeFramePtr input, Size size) {
				output.SetValue(
					input->GetSize(),
					input->GetType(),
					[input](ComputeFramePtr output) {
						functions::Resize(input, output);
					}
				);
			}
		);
	/*
	return ComputeTaskHelper::Make([input, output, size]() {
		ComputeFramePtr inputFrame = *input;
		Size sizeValue = *size;

		output.SetValue(
			sizeValue,
			inputFrame->GetType(),
			[inputFrame](ComputeFramePtr outputFrame) {
				functions::Resize(inputFrame, outputFrame);
			}
		);	
		
	});
	*/
}

void ResizeComputeNode::SetSize(Size size)
{
	_size.DefaultValue = size;
}

} // End frox