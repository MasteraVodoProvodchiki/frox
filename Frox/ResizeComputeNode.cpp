#include "ResizeComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>
#include <algorithm>

namespace frox {

namespace functions {

#ifndef WITHOUT_OPENCV
void Resize(ComputeFramePtr input, ComputeFramePtr output)
{
	assert(input->IsOpencv() && output->IsOpencv());
	assert(input->GetType() == output->GetType());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());

	cv::Mat inputMat = cvInput->GetMat();
	cv::Mat outputMat = cvOuput->GetMat();

	// Convert
	// TODO. Add fx/fy and interpolation
	EComputeFrameType type = input->GetType().Type;
	switch (type)
	{
	case ECFT_Bool:
	case ECFT_UInt8:
	case ECFT_UInt16:
	case ECFT_Float:
		cv::resize(inputMat, outputMat, outputMat.size());
		break;
	case ECFT_UInt32:
		cv::resize(inputMat, outputMat, outputMat.size(), 0, 0, cv::INTER_LINEAR_EXACT);
		break;
	default:
		assert(false);
		break;
	}
	
}
#else
void Resize(ComputeFramePtr input, ComputeFramePtr output)
{
	// TODO. Add solution without opencv
	assert(false);
}
#endif // WITHOUT_OPENCV

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
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto size = _size.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	// Make task
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
					size,
					input->GetType(),
					[input](ComputeFramePtr output) {
						functions::Resize(input, output);
					}
				);
			}
		);
}

void ResizeComputeNode::SetSize(Size size)
{
	_size.DefaultValue = size;
}

} // End frox