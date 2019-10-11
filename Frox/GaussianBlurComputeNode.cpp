#include "GaussianBlurComputeNode.h"
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

int TypeToOpenCV(EGaussianBlurBorderType type)
{
	switch (type)
	{
	case EGaussianBlurBorderType::Constant:
		return cv::BORDER_CONSTANT;
	case EGaussianBlurBorderType::Replicate:
		return cv::BORDER_REPLICATE;
	case EGaussianBlurBorderType::Reflect:
		return cv::BORDER_REFLECT;
	case EGaussianBlurBorderType::Wrap:
		return cv::BORDER_WRAP;
	case EGaussianBlurBorderType::Reflect101:
		return cv::BORDER_REFLECT_101;
	case EGaussianBlurBorderType::Transparent:
		return cv::BORDER_TRANSPARENT;
	case EGaussianBlurBorderType::Default:
		return cv::BORDER_DEFAULT;
	case EGaussianBlurBorderType::Isolated:
		return cv::BORDER_ISOLATED;
	default:
		return cv::BORDER_DEFAULT;
	}
}

void GaussianBlur(ComputeFramePtr input, ComputeFramePtr output, Size size, double sigmaX, double sigmaY, EGaussianBlurBorderType type)
{
	assert(input->IsOpencv() && output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());

	cv::Mat inputMat = cvInput->GetMat();
	cv::Mat outputMat = cvOuput->GetMat();

	// Convert
	cv::GaussianBlur(inputMat, outputMat, cv::Size(size.Width, size.Height), sigmaX, sigmaY, TypeToOpenCV(type));
}
#else
void GaussianBlur(ComputeFramePtr input, ComputeFramePtr output, Size size, double sigmaX, double sigmaY, EGaussianBlurBorderType type)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(GaussianBlurComputeNode)

GaussianBlurComputeNode::GaussianBlurComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _ksize("ksize", Size{ 3, 3 })
	, _sigmaX("sigmaX", 0.0)
	, _sigmaY("sigmaY", 0.0)
	, _borderType(EGaussianBlurBorderType::Reflect101)
	, _output("output")
{}

GaussianBlurComputeNode::~GaussianBlurComputeNode()
{}

void GaussianBlurComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_ksize);
	RegisterInput(&_sigmaX);
	RegisterInput(&_sigmaY);
	RegisterOutput(&_output);
}

bool GaussianBlurComputeNode::IsValid() const
{
	return true;
}

ComputeTask* GaussianBlurComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto ksize = _ksize.GetValue(inputData);
	auto sigmaX = _sigmaX.GetValue(inputData);
	auto sigmaY = _sigmaY.GetValue(inputData);
	auto borderType = _borderType;
	auto output = _output.GetValue(outputData);
	
	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, ksize, sigmaX, sigmaY, borderType)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, Size ksize, double sigmaX, double sigmaY, EGaussianBlurBorderType borderType) {
				return input != nullptr && input->IsValid();
			},
			[output](ComputeFramePtr input, Size ksize, double sigmaX, double sigmaY, EGaussianBlurBorderType borderType) {
				output.SetValue(
					input->GetSize(),
					ComputeFrameType{ EComputeFrameType::ECFT_UInt8, input->GetChannels() },
					[input, ksize, sigmaX, sigmaY, borderType](ComputeFramePtr output) {
						functions::GaussianBlur(input, output, ksize, sigmaX, sigmaY, borderType);
					}
				);
			}
		);
}

void GaussianBlurComputeNode::SetKernelSize(Size ksize)
{
	_ksize = ksize;
}

void GaussianBlurComputeNode::SetSigmaX(double sigmaX)
{
	_sigmaX = sigmaX;
}

void GaussianBlurComputeNode::SetSigmaY(double sigmaY)
{
	_sigmaY = sigmaY;
}

void GaussianBlurComputeNode::SetBorderType(EGaussianBlurBorderType borderType)
{
	_borderType = borderType;
}

} // End frox