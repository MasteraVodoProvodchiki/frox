#include "AvgComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(AvgComputeNode)

namespace functions {

void Avg(ComputeFramePtr input, ComputeFramePtr output)
{
	EComputeFrameType type = input->GetType();
	Size size = input->GetSize();
	uint32_t nbElemenents = size.Width * size.Height;

	switch (type)
	{
	case ECFT_Bool: {
		uint32_t sum = 0;
		utils::Foreach<bool>(input, [&sum](bool value) {
			sum += value ? 1 : 0;
		});
		uint8_t avg = uint8_t(sum / nbElemenents);

		output->At<bool>(0, 0) = avg > 127;
		break;
	}
	case ECFT_UInt8: {
		uint32_t sum = 0;
		utils::Foreach<uint8_t>(input, [&sum](uint8_t value) {
			sum += value;
		});
		uint8_t avg = uint8_t(sum / nbElemenents);

		output->At<uint8_t>(0, 0) = avg;
		break;
	}
	case ECFT_UInt16: {
		uint32_t sum = 0;
		utils::Foreach<uint16_t>(input, [&sum](uint16_t value) {
			sum += value;
		});
		uint16_t avg = uint16_t(sum / nbElemenents);

		output->At<uint16_t>(0, 0) = avg;
		break;
	}
	case ECFT_UInt32: {
		uint64_t sum = 0;
		utils::Foreach<uint32_t>(input, [&sum](uint32_t value) {
			sum += value;
		});
		uint32_t avg = uint32_t(sum / nbElemenents);

		output->At<uint32_t>(0, 0) = avg;
		break;
	}
	case ECFT_Float: {
		double sum = 0;
		utils::Foreach<float>(input, [&sum](float value) {
			sum += value;
		});
		float avg = float(sum / double(nbElemenents));

		output->At<float>(0, 0) = avg;
		break;
	}
	default:
		break;
	}
}

} // End functions

AvgComputeNode::AvgComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	// , _input(0)
	// , _output(0)
{}

AvgComputeNode::~AvgComputeNode()
{}

void AvgComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
	// _input = CreateInput("input");
	// _output = CreateOutput("output");
}

/*
void AvgComputeNode::OnInputChanged(uint32_t inId, ComputeFramePtr frame)
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);
	if (input && !output)
	{
		output = FroxInstance()->CreateComputeFrame(Size{1 ,1}, input->GetType());
		SetOutput(_output, output);
	}
}
*/

bool AvgComputeNode::IsValid() const
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);

	return
		input != nullptr &&
		output != nullptr &&
		input->GetType() == output->GetType();
		output->GetSize() == Size{1, 1};
}

ComputeTask* AvgComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// ComputeFramePtr input = GetInput(_input);
	// ComputeFramePtr output = GetOutput(_output);

	auto input = _input.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	return ComputeTaskUtils::Make([input, output]() {
		
		ComputeFramePtr inputFrame = *input;
		// Check

		output.SetValue(
			Size{ 1 ,1 },
			inputFrame->GetType(),
			[inputFrame](ComputeFramePtr outputFrame) {
				functions::Avg(inputFrame, outputFrame);
			}
		);
	});
}

} // End frox