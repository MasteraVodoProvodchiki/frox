#include "CropComputeNode.h"
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
void Crop(ComputeFramePtr input, ComputeFramePtr output, Rect rect)
{
	assert(input->GetType() == output->GetType());
	assert(output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());
	cv::Mat inputMat = cvInput->GetMat();
	cv::Mat outputMat = cvOuput->GetMat();

	// Crop
	inputMat(cv::Rect(rect.X, rect.Y, rect.Width, rect.Height)).copyTo(outputMat);
}
#else
void Crop(ComputeFramePtr input, ComputeFramePtr output, Rect rect)
{
	uint32_t inColumn = rect.X;
	uint32_t inRow = rect.Y;

	uint32_t width = rect.Width;
	uint32_t height = rect.Height;

	uint32_t elementSize = input->GetElementSize();

	for (uint32_t outRow = 0; outRow < height; ++outRow)
	{
		const uint8_t* srcRowData = input->GetRowData<uint8_t>(outRow + inRow);
		uint8_t* dstRowData = output->GetRowData<uint8_t>(outRow);
		memcpy(dstRowData, srcRowData + inColumn * elementSize, width * elementSize);
	}
}
#endif // WITHOUT_OPENCV

} // End functins

FROX_COMPUTENODE_IMPL(CropComputeNode)

CropComputeNode::CropComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _offset("offset", Point{ 0, 0 })
	, _size("size", Size{ 0, 0 })
	, _output("output")
{}

CropComputeNode::~CropComputeNode()
{}

void CropComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_offset);
	RegisterInput(&_size);
	RegisterOutput(&_output);
}

bool CropComputeNode::IsValid() const
{
	return true;
}

ComputeTask* CropComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Preapre inputs/output
	auto input = _input.GetValue(inputData);
	auto offset = _offset.GetValue(inputData);
	auto size = _size.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, offset, size)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, Point offset, Size size) {
				return input != nullptr && input->IsValid() && size.IsValid();
			},
			[output](ComputeFramePtr input, Point offset, Size size) {
				Size inputSize = input->GetSize();
				Rect rect = Rect(offset, size);

				// min W/H
				int32_t width = std::min(rect.Width, int32_t(inputSize.Width) - rect.X);
				int32_t height = std::min(rect.Height, int32_t(inputSize.Height) - rect.Y);

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
}

void CropComputeNode::SetOffset(Point offset)
{
	_offset = offset;
}

void CropComputeNode::SetSize(Size size)
{
	_size = size;
}

} // End frox