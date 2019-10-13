#include "InRangeComputeNode.h"
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
void InRange(ComputeFramePtr input, ComputeFramePtr output, double low, double high)
{
	assert(input->IsOpencv() && output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());

	cv::Mat inputMat = cvInput->GetMat();
	cv::Mat outputMat = cvOuput->GetMat();

	// Convert
	cv::Scalar minTresh(low);
	cv::Scalar maxTresh(high);
	cv::inRange(inputMat, minTresh, maxTresh, outputMat);
}
#else
void ConvertTo(ComputeFramePtr input, ComputeFramePtr output, double alpha, double beta)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(InRangeComputeNode)

InRangeComputeNode::InRangeComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _low("low", 1.0)
	, _high("high", 0.0)
	, _output("output")
{}

InRangeComputeNode::~InRangeComputeNode()
{}

void InRangeComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_low);
	RegisterInput(&_high);
	RegisterOutput(&_output);
}

bool InRangeComputeNode::IsValid() const
{
	return true;
}

ComputeTask* InRangeComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto low = _low.GetValue(inputData);
	auto high = _high.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	
	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, low, high)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, double low, double high) {
				return input != nullptr && input->IsValid() && low < high;
			},
			[output](ComputeFramePtr input, double low, double high) {
				output.SetValue(
					input->GetSize(),
					ComputeFrameType{ EComputeFrameType::ECFT_UInt8, input->GetChannels() },
					[input, low, high](ComputeFramePtr output) {
						functions::InRange(input, output, low, high);
					}
				);
			}
		);
}

void InRangeComputeNode::SetLow(double low)
{
	_low = low;
}

void InRangeComputeNode::SetHigh(double high)
{
	_high = high;
}

} // End frox