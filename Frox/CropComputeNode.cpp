#include "CropComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
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
	, _input("input")
	, _rect("rect", Rect{ -1, -1, -1, -1 })
	, _output("output")
	
{}

CropComputeNode::~CropComputeNode()
{}

void CropComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_rect);
	RegisterOutput(&_output);
}

bool CropComputeNode::IsValid() const
{
	return true;
}

ComputeTask* CropComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	auto input = _input.GetValue(inputData);
	auto rect = _rect.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	return
		ComputeTaskHelper::UnPackProps(input, rect)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, Rect rect) {
				return input != nullptr && input->IsValid() && rect.IsValid();
			},
			[output](ComputeFramePtr input, Rect rect) {
				Size inputSize = input->GetSize();

				// min W/H
				int32_t width = std::min(rect.Width, int32_t(inputSize.Width) - rect.X);
				int32_t height = std::min(rect.Heihgt, int32_t(inputSize.Height) - rect.Y);

				Size outputSize = Size{ uint32_t(width), uint32_t(height) };

				output.SetValue(
					outputSize,
					input->GetType(),
					[input, rect](ComputeFramePtr output) {
						functions::Crop(input, output, rect);
					}
				);	
			}
		);
	/*
	return ComputeTaskHelper::Make([input, output, rect]() {
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
	*/
}

void CropComputeNode::SetRect(Rect rect)
{
	_rect = rect;
}

} // End frox