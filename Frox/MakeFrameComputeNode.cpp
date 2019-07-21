#include "MakeFrameComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>

namespace frox {

namespace utils {

template<typename ValueT>
struct StaticValue
{
	ValueT Value;
	StaticValue(ValueT value)
		: Value(value)
	{}

	template<typename T>
	T operator ()(const T&) const
	{
		return T(Value);
	}
};

struct RandomValue
{
	template<typename T>
	T operator ()(const T&) const
	{
		return T(std::rand() % std::numeric_limits<T>::max());
	}

	template<>
	bool operator ()(const bool&) const
	{
		return (std::rand() % 1) ? true : false;
	}

	template<>
	float operator ()(const float&) const
	{
		return float(std::rand()) / float(RAND_MAX);
	}
};

} // End utils

namespace functions {

#ifndef WITHOUT_OPENCV
void Fill(ComputeFramePtr output, Variant value)
{
	assert( output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());
	cv::Mat outputMat = cvOuput->GetMat();

	// Set
	ComputeFrameType type = output->GetType();
	switch (type.Type)
	{
	case ECFT_Bool: {
		bool boolValue = value.To<bool>();
		outputMat.setTo(cv::Scalar(boolValue, boolValue, boolValue, boolValue));
		break;
	}
	case ECFT_UInt8: {
		uint8_t intValue = value.To<uint8_t>();
		outputMat.setTo(cv::Scalar(intValue, intValue, intValue, intValue));
		break;
	}
	case ECFT_UInt16: {
		uint16_t intValue = value.To<uint16_t>();
		outputMat.setTo(cv::Scalar(intValue, intValue, intValue, intValue));
		break;
	}
	case ECFT_UInt32: {
		uint32_t intValue = value.To<uint32_t>();
		outputMat.setTo(cv::Scalar(intValue, intValue, intValue, intValue));
		break;
	}
	case ECFT_Float: {
		float floatValue = value.To<float>();
		outputMat.setTo(cv::Scalar(floatValue, floatValue, floatValue, floatValue));
		break;
	}
	default:
		break;
	}
}
#else
void Fill(ComputeFramePtr output, Variant value)
{
	ComputeFrameType type = output->GetType();
	switch (type.Type)
	{
	case ECFT_Bool: {
		bool boolValue = value.To<bool>();
		utils::Fill<bool>(output, utils::StaticValue<bool>(boolValue));
		break;
	}
	case ECFT_UInt8: {
		uint8_t intValue = value.To<uint8_t>();
		utils::Fill<uint8_t>(output, utils::StaticValue<uint8_t>(intValue));
		break;
	}
	case ECFT_UInt16: {
		uint16_t intValue = value.To<uint16_t>();
		utils::Fill<uint16_t>(output, utils::StaticValue<uint16_t>(intValue));
		break;
	}
	case ECFT_UInt32: {
		uint32_t intValue = value.To<uint32_t>();
		utils::Fill<uint32_t>(output, utils::StaticValue<uint32_t>(intValue));
		break;
	}
	case ECFT_Float: {
		float floatValue = value.To<float>();
		utils::Fill<float>(output, utils::StaticValue<float>(floatValue));
		break;
	}
	default:
		break;
	}
}
#endif // End WITHOUT_OPENCV

template <typename CallbackT>
void Fill(ComputeFramePtr output, CallbackT callback)
{
	ComputeFrameType type = output->GetType();
	switch (type.Type)
	{
	case ECFT_Bool: {
		utils::Fill<bool>(output, callback);
		break;
	}
	case ECFT_UInt8: {
		utils::Fill<uint8_t>(output, callback);
		break;
	}
	case ECFT_UInt16: {
		utils::Fill<uint16_t>(output, callback);
		break;
	}
	case ECFT_UInt32: {
		utils::Fill<uint32_t>(output, callback);
		break;
	}
	case ECFT_Float: {
		utils::Fill<float>(output, callback);
		break;
	}
	default:
		break;
	}
}

void FillRandom(ComputeFramePtr output)
{
#ifndef WITHOUT_OPENCV
	assert(output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());

	cv::Mat outputMat = cvOuput->GetMat();

	ComputeFrameType type = output->GetType();
	switch (type.Type)
	{
	case ECFT_Bool: {
		cv::randu(outputMat, cv::Scalar(0), cv::Scalar(1 + 1));
		break;
	}
	case ECFT_UInt8: {
		cv::randu(outputMat, cv::Scalar(0), cv::Scalar(uint32_t(std::numeric_limits<uint8_t>::max()) + 1));
		break;
	}
	case ECFT_UInt16: {
		cv::randu(outputMat, cv::Scalar(0), cv::Scalar(uint32_t(std::numeric_limits<uint16_t>::max()) + 1));
		break;
	}
	case ECFT_UInt32: {
		cv::randu(outputMat, cv::Scalar(0), cv::Scalar(std::numeric_limits<uint32_t>::max()));
		break;
	}
	case ECFT_Float: {
		cv::randu(outputMat, cv::Scalar(0.f), cv::Scalar(1.f));
		break;
	}
	default:
		assert(false);
		break;
	}
#else
	functions::Fill(output, utils::RandomValue());
#endif
}

} // End functions

/// Base
MakeFrameBaseComputeNode::MakeFrameBaseComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _width("width", 1)
	, _height("height", 1)
	, _type(EComputeFrameType::ECFT_None)
	, _channels("channels", 1)
	, _output("output")
{}

MakeFrameBaseComputeNode::~MakeFrameBaseComputeNode()
{}

void MakeFrameBaseComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_width);
	RegisterInput(&_height);
	RegisterInput(&_channels);
	RegisterOutput(&_output);
}

bool MakeFrameBaseComputeNode::IsValid() const
{
	return true;
}

void MakeFrameBaseComputeNode::SetWidth(uint32_t width)
{
	_width = width;
}

void MakeFrameBaseComputeNode::SetHeight(uint32_t height)
{
	_height = height;
}

void MakeFrameBaseComputeNode::SetType(EComputeFrameType type)
{
	_type = type;
}

void MakeFrameBaseComputeNode::SetChannels(uint32_t channels)
{
	_channels = channels;
}

/// Make By Value
MakeFrameComputeNode::MakeFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

bool MakeFrameComputeNode::IsValid() const
{
	return _value.IsValid();
}

ComputeTask* MakeFrameComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/ouput
	Variant value = _value;

	auto width = _width.GetValue(inputData);
	auto height = _height.GetValue(inputData);
	auto type = _type;
	auto channels = _channels.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	
	// Make task
	return
		ComputeTaskHelper::UnPackProps(width, height, type, channels)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](uint32_t width, uint32_t height, EComputeFrameType type, uint32_t channels) {
				return
					width > 0 && height > 0 &&
					type != EComputeFrameType::ECFT_None &&
					channels > 0;
			},
			[value, output](uint32_t width, uint32_t height, EComputeFrameType type, uint32_t channels) {
				output.SetValue(
					Size{ width, height },
					ComputeFrameType{ type, channels },
					[value](ComputeFramePtr output) {
						functions::Fill(output, value);
					}
				);
			}
		);
}

void MakeFrameComputeNode::SetValue(Variant value)
{
	_value = value;
}

/// Make By Zero
MakeZeroFrameComputeNode::MakeZeroFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{
	SetType(EComputeFrameType::ECFT_Float);
	SetValue(0.f);
}

/// Make By Noise
MakeNoiseFrameComputeNode::MakeNoiseFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

ComputeTask* MakeNoiseFrameComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto width = _width.GetValue(inputData);
	auto height = _height.GetValue(inputData);
	auto type = _type;
	auto channels = _channels.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	
	// Make task
	return
		ComputeTaskHelper::UnPackProps(width, height, type, channels)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](uint32_t width, uint32_t height, EComputeFrameType type, uint32_t channels) {
				return
					width > 0 && height > 0 &&
					type != EComputeFrameType::ECFT_None &&
					channels > 0;
			},
			[output](uint32_t width, uint32_t height, EComputeFrameType type, uint32_t channels) {
				output.SetValue(
					Size{ width, height },
					ComputeFrameType{ type, channels },
					[](ComputeFramePtr output) {
						functions::FillRandom(output);
					}
				);
			}
		);
}

/// UEImageReaderComputeNode
ConstFrameComputeNode::ConstFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _output("output")
{}

ConstFrameComputeNode::~ConstFrameComputeNode()
{}

void ConstFrameComputeNode::AllocateDefaultPins()
{
	RegisterOutput(&_output);
}

bool ConstFrameComputeNode::IsValid() const
{
	return true;
}

ComputeTask* ConstFrameComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	auto frame = _frame;
	auto output = _output.GetValue(outputData);

	return ComputeTaskHelper::Make([frame, output]() {
		output.SetValue(frame);
	});
}

void ConstFrameComputeNode::SetFrame(ComputeFramePtr frame)
{
	_frame = frame;
}

FROX_COMPUTENODE_IMPL(ConstFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeZeroFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeNoiseFrameComputeNode)

} // End frox