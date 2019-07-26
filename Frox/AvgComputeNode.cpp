#include "AvgComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(AvgComputeNode)

namespace functions {

#ifndef WITHOUT_OPENCV
template<typename T>
T Mean(ComputeFramePtr frame)
{
	assert(frame->IsOpencv());

	OpenCVComputeFrameImpl* cvFrame = reinterpret_cast<OpenCVComputeFrameImpl*>(frame.get());
	cv::Scalar value = cv::mean(cvFrame->GetMat());

	return T(value.val[0]);
}
#else
template<typename T>
T Mean(ComputeFramePtr input)
{
	ComputeFrameType type = input->GetType();
	Size size = input->GetSize();
	uint32_t nbElemenents = size.Width * size.Height;

	switch (type.Type)
	{
	case ECFT_Bool: {
		uint32_t sum = 0;
		utils::Foreach<bool>(input, [&sum](bool value) {
			sum += value ? 1 : 0;
		});
		uint8_t avg = uint8_t(sum / nbElemenents);

		return T(avg > 127);
	}
	case ECFT_UInt8: {
		uint32_t sum = 0;
		utils::Foreach<uint8_t>(input, [&sum](uint8_t value) {
			sum += value;
		});
		return T(sum / nbElemenents);
	}
	case ECFT_UInt16: {
		uint32_t sum = 0;
		utils::Foreach<uint16_t>(input, [&sum](uint16_t value) {
			sum += value;
		});
		return T(sum / nbElemenents);
	}
	case ECFT_UInt32: {
		uint64_t sum = 0;
		utils::Foreach<uint32_t>(input, [&sum](uint32_t value) {
			sum += value;
		});
		return T(sum / nbElemenents);
	}
	case ECFT_Float: {
		double sum = 0;
		utils::Foreach<float>(input, [&sum](float value) {
			sum += value;
		});
		return T(sum / double(nbElemenents));
	}
	default:
		assert(false);
		break;
	}

	return 0;
}
#endif // WITHOUT_OPENCV

void Avg(ComputeFramePtr input, ComputeFramePtr output)
{
	ComputeFrameType inputType = input->GetType();
	ComputeFrameType outputType = output->GetType();
	assert(inputType == outputType);

	Size size = input->GetSize();
	uint32_t nbElemenents = size.Width * size.Height;

	switch (inputType.Type)
	{
	case ECFT_Bool: {
		output->At<bool>(0, 0) = Mean<bool>(input);
		break;
	}
	case ECFT_UInt8: {
		output->At<uint8_t>(0, 0) = Mean<uint8_t>(input);
		break;
	}
	case ECFT_UInt16: {
		output->At<uint16_t>(0, 0) = Mean<uint16_t>(input);
		break;
	}
	case ECFT_UInt32: {
		output->At<uint32_t>(0, 0) = Mean<uint32_t>(input);
		break;
	}
	case ECFT_Float: {
		output->At<float>(0, 0) = Mean<float>(input);
		break;
	}
	default:
		break;
	}
}

} // End functions

AvgComputeNode::AvgComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _output("output")
{}

AvgComputeNode::~AvgComputeNode()
{}

void AvgComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
}

bool AvgComputeNode::IsValid() const
{
	return true;
}

ComputeTask* AvgComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare input/ouput
	auto input = _input.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	// Make task
	return ComputeTaskHelper::UnPackProps(input)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input) {
				return input != nullptr && input->IsValid();
			},
			[output](ComputeFramePtr input) {
				output.SetValue(
					Size{ 1 ,1 },
					input->GetType(),
					[input](ComputeFramePtr output) {
						functions::Avg(input, output);
					}
				);
			}
		);
}

} // End frox