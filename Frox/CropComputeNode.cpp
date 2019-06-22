#include "CropComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>
#include <algorithm>

namespace frox {

FROX_COMPUTENODE_IMPL(CropComputeNode)

CropComputeNode::CropComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input(0)
	, _output(0)
	, _rect({-1, -1, -1, -1})
{}

CropComputeNode::~CropComputeNode()
{}

void CropComputeNode::AllocateDefaultPins()
{
	_input = CreateInput("input");
	_output = CreateOutput("output");
}

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

bool CropComputeNode::IsValid() const
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);

	return
		input != nullptr &&
		output != nullptr &&
		input->GetType() == output->GetType() &&
		_rect.IsValid() &&
		input->IsValid();
}

ComputeTask* CropComputeNode::CreateComputeTask()
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);
	Rect rect = _rect;

	return ComputeTaskUtils::Make([input, output, rect]() {

		uint32_t inColumn = rect.X;
		uint32_t inRow = rect.Y;

		uint32_t width = rect.Width;
		uint32_t height = rect.Heihgt;

		uint32_t elementSize = input->GetElementSize();

		for (uint32_t outRow=0; outRow < height; ++outRow)
		{
			const uint8_t* srcRowData = input->GetRowData<uint8_t>(outRow + inRow);
			uint8_t* dstRowData = output->GetRowData<uint8_t>(outRow);
			memcpy(dstRowData, srcRowData + inColumn * elementSize, width * elementSize);
		}
	});
}

void CropComputeNode::SetRect(Rect rect)
{
	_rect = rect;
}

} // End frox