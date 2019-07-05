#include "MakeFrameComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

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

/// Base
MakeFrameBaseComputeNode::MakeFrameBaseComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _width(1)
	, _height(1)
	, _type(EComputeFrameType::ECFT_None)
	, _channels(1)
{}

MakeFrameBaseComputeNode::~MakeFrameBaseComputeNode()
{}

void MakeFrameBaseComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_width);
	RegisterInput(&_height);
	RegisterInput(&_channels);
	RegisterOutput(&_output);
	// _output = CreateOutput("output");
}

/*
void MakeFrameBaseComputeNode::OnPostInit()
{
	if (_width > 0 && _height > 0 && _type != EComputeFrameType::ECFT_None)
	{
		ComputeFramePtr output = FroxInstance()->CreateComputeFrame(Size{ _width, _height }, _type, _channels);
		SetOutput(_output, output);
	}
}
*/

bool MakeFrameBaseComputeNode::IsValid() const
{
	/*
	return
		_width > 0 && _height > 0 &&
		_type != EComputeFrameType::ECFT_None &&
		_channels > 0 &&
		GetOutput(_output) != nullptr;
	*/
	return false;
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
	/*
	return
		Super::IsValid() &&
		_value.IsValid();
	*/
	return false;
}

ComputeTask* MakeFrameComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	Variant value = _value;
	// ComputeFramePtr output = GetOutput(_output);
	auto width = _width.GetValue(inputData);
	auto height = _height.GetValue(inputData);
	auto channels = _channels.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	auto type = _type;

	return ComputeTaskUtils::Make([width, height, type, channels, value, output]() {
		uint32_t widthValue = *width;
		uint32_t heightValue = *height;
		uint32_t channelsValue = *channels;

		output.SetValue(
			Size{ widthValue, heightValue },
			ComputeFrameType{ type, channelsValue },
			[value](ComputeFramePtr output) {
				ComputeFrameType type = output->GetType();
				Size size = output->GetSize();
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
		);
	});
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
	// ComputeFramePtr output = GetOutput(_output);
	auto width = _width.GetValue(inputData);
	auto height = _height.GetValue(inputData);
	auto channels = _channels.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	auto type = _type;

	return ComputeTaskUtils::Make([width, height, type, channels, output]() {
		uint32_t widthValue = *width;
		uint32_t heightValue = *height;
		uint32_t channelsValue = *channels;

		output.SetValue(
			Size{ widthValue, heightValue },
			ComputeFrameType{ type, channelsValue },
			[](ComputeFramePtr output) {
				ComputeFrameType type = output->GetType();
				Size size = output->GetSize();
				switch (type.Type)
				{
					case ECFT_Bool: {
						utils::Fill<bool>(output, utils::RandomValue());
						break;
					}
					case ECFT_UInt8: {
						utils::Fill<uint8_t>(output, utils::RandomValue());
						break;
					}
					case ECFT_UInt16: {
						utils::Fill<uint16_t>(output, utils::RandomValue());
						break;
					}
					case ECFT_UInt32: {
						utils::Fill<uint32_t>(output, utils::RandomValue());
						break;
					}
					case ECFT_Float: {
						utils::Fill<float>(output, utils::RandomValue());
						break;
					}
					default:
						break;
				}
			}
		);	
		
	});
}

/// UEImageReaderComputeNode
ConstFrameComputeNode::ConstFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

ConstFrameComputeNode::~ConstFrameComputeNode()
{}

void ConstFrameComputeNode::AllocateDefaultPins()
{
	RegisterOutput(&_output);
	// _output = CreateOutput("output");
}

/*
void ConstFrameComputeNode::OnPostInit()
{
	if (_frame)
	{
		SetOutput(_output, _frame);
	}
}
*/

bool ConstFrameComputeNode::IsValid() const
{
	// return GetOutput(_output) != nullptr;
	return false;
}

ComputeTask* ConstFrameComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// ComputeFramePtr output = GetOutput(_output);
	auto output = _output.GetValue(outputData);

	return ComputeTaskUtils::Make([output]() {
		// Nothing
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