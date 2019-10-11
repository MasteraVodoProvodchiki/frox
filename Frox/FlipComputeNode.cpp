#include "FlipComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"

#include "Frox.h"
#include "Utils.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>

namespace frox {

namespace functions {

#ifndef WITHOUT_OPENCV
int FlagsToFlipCode(bool vertical, bool horizontal)
{
	assert(vertical || horizontal);

	if (vertical && horizontal)
	{
		return -1;
	}
	if (vertical)
	{
		return 0;
	}

	return 1;
}

void Flip(ComputeFramePtr input, ComputeFramePtr output, bool vertical, bool horizontal)
{
	assert(input->IsOpencv() && output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());

	cv::Mat inputMat = cvInput->GetMat();
	cv::Mat outputMat = cvOuput->GetMat();

	// Convert
	if (vertical || horizontal)
	{
		cv::flip(inputMat, outputMat, FlagsToFlipCode(vertical, horizontal));
	}
	else
	{
		inputMat.copyTo(outputMat);
	}
}
#else
void Flip(ComputeFramePtr input, ComputeFramePtr output, bool vertical, bool horizontal)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(FlipComputeNode)

FlipComputeNode::FlipComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _vertical("vertical", true)
	, _horizontal("horizontal", true)
	, _output("output")
{}

FlipComputeNode::~FlipComputeNode()
{}

void FlipComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_vertical);
	RegisterInput(&_horizontal);
	RegisterOutput(&_output);
}

bool FlipComputeNode::IsValid() const
{
	return true;
}

ComputeTask* FlipComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto vertical = _vertical.GetValue(inputData);
	auto horizontal = _horizontal.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	
	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, vertical, horizontal)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, bool vertical, bool horizontal) {
				return input != nullptr && input->IsValid();
			},
			[output](ComputeFramePtr input, bool vertical, bool horizontal) {
				output.SetValue(
					input->GetSize(),
					input->GetType(),
					[input, vertical, horizontal](ComputeFramePtr output) {
						functions::Flip(input, output, vertical, horizontal);
					}
				);
			}
		);
}

void FlipComputeNode::SetVertical(bool vertical)
{
	_vertical = vertical;
}

void FlipComputeNode::SetHorizontal(bool horizontal)
{
	_horizontal = horizontal;
}

} // End frox