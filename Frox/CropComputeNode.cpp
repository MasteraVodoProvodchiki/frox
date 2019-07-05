#include "CropComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>
#include <algorithm>

namespace frox {

namespace functions {

void Crop(ComputeFramePtr input, ComputeFramePtr output, Rect rect)
{
	uint32_t inColumn = rect.X;
	uint32_t inRow = rect.Y;

	uint32_t width = rect.Width;
	uint32_t height = rect.Heihgt;

	uint32_t elementSize = input->GetElementSize();

	for (uint32_t outRow = 0; outRow < height; ++outRow)
	{
		const uint8_t* srcRowData = input->GetRowData<uint8_t>(outRow + inRow);
		uint8_t* dstRowData = output->GetRowData<uint8_t>(outRow);
		memcpy(dstRowData, srcRowData + inColumn * elementSize, width * elementSize);
	}
}

} // End functins

FROX_COMPUTENODE_IMPL(CropComputeNode)

CropComputeNode::CropComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	// , _input(0)
	// , _output(0)
	, _rect(Rect{-1, -1, -1, -1})
{}

CropComputeNode::~CropComputeNode()
{}

void CropComputeNode::AllocateDefaultPins()
{
	// _input = CreateInput("input");
	// _output = CreateOutput("output");
	RegisterInput(&_input);
	RegisterInput(&_rect);
	RegisterOutput(&_output);
}

/*
void CropComputeNode::OnPostInit()
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);
	if (input && !output)
	{
		Size inputSize = input->GetSize();

		// min W/H
		int32_t width = std::min(_rect.Width, int32_t(inputSize.Width) - _rect.X);
		int32_t height = std::min(_rect.Heihgt, int32_t(inputSize.Height) - _rect.Y);
		
		Size outputSize = Size{uint32_t(width), uint32_t(height)};
		ComputeFramePtr output = FroxInstance()->CreateComputeFrame(outputSize, input->GetType());
		SetOutput(_output, output);
	}
}
*/

bool CropComputeNode::IsValid() const
{
	/*
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);

	return
		input != nullptr &&
		output != nullptr &&
		input->GetType() == output->GetType() &&
		_rect.IsValid() &&
		input->IsValid();
	*/
	return false;
}

ComputeTask* CropComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// ComputeFramePtr input = GetInput(_input);
	// ComputeFramePtr output = GetOutput(_output);
	// Rect rect = _rect;
	auto input = _input.GetValue(inputData);
	auto rect = _rect.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	return ComputeTaskUtils::Make([input, output, rect]() {
		ComputeFramePtr inputFrame = *input;
		Rect rectValue = *rect;

		Size inputSize = inputFrame->GetSize();

		// min W/H
		int32_t width = std::min(rectValue.Width, int32_t(inputSize.Width) - rectValue.X);
		int32_t height = std::min(rectValue.Heihgt, int32_t(inputSize.Height) - rectValue.Y);

		Size outputSize = Size{ uint32_t(width), uint32_t(height) };

		output.SetValue(
			outputSize,
			inputFrame->GetType(),
			[inputFrame, rectValue](ComputeFramePtr outputFrame) {
				functions::Crop(inputFrame, outputFrame, rectValue);
			}
		);		
	});
}

void CropComputeNode::SetRect(Rect rect)
{
	_rect = rect;
}

} // End frox