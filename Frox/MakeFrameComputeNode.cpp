#include "MakeFrameComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

namespace utils {

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
{}

MakeFrameBaseComputeNode::~MakeFrameBaseComputeNode()
{}

void MakeFrameBaseComputeNode::AllocateDefaultPins()
{
	_output = CreateOutput("output");
}

void MakeFrameBaseComputeNode::OnPostInit()
{
	if (_width > 0 && _height > 0 && _type != EComputeFrameType::ECFT_None)
	{
		ComputeFramePtr output = FroxInstance()->CreateComputeFrame(Size{ _width, _height }, _type);
		SetOutput(_output, output);
	}
}

bool MakeFrameBaseComputeNode::IsValid() const
{
	return
		_width > 0 && _height > 0 &&
		_type != EComputeFrameType::ECFT_None &&
		GetOutput(_output) != nullptr;
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


/// Make By Value
MakeFrameComputeNode::MakeFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

bool MakeFrameComputeNode::IsValid() const
{
	return
		Super::IsValid() &&
		_value.IsValid();
}

ComputeTask* MakeFrameComputeNode::CreateComputeTask()
{
	Variant value = _value;

	ComputeFramePtr output = GetOutput(_output);

	return ComputeTaskUtils::Make([value, output]() {
		EComputeFrameType type = output->GetType();
		Size size = output->GetSize();
		switch (type)
		{
		case ECFT_Bool: {
			bool boolValue = value.To<bool>();
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				bool* values = output->GetRowData<bool>(row);
				memset(values, boolValue, size.Width * sizeof(bool));
			}
			break;
		}
		case ECFT_UInt8: {
			int intValue = value.To<int>();
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				uint8_t* values = output->GetRowData<uint8_t>(row);
				memset(values, uint8_t(intValue), size.Width * sizeof(uint8_t));
			}
			break;
		}
		case ECFT_UInt16: {
			int intValue = value.To<int>();
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				uint16_t* values = output->GetRowData<uint16_t>(row);
				memset(values, uint16_t(intValue), size.Width * sizeof(uint16_t));
			}
			break;
		}
		case ECFT_UInt32: {
			int intValue = value.To<int>();
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				uint32_t* values = output->GetRowData<uint32_t>(row);
				memset(values, uint32_t(intValue), size.Width * sizeof(uint32_t));
			}
			break;
		}
		case ECFT_Float: {
			float floatValue = value.To<float>();
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				float* values = output->GetRowData<float>(row);
				for (uint32_t column = 0; column < size.Width; ++column)
				{
					float& out = *(values + column);
					out = floatValue;
				}
			}
			break;
		}
		default:
			break;
		}
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

ComputeTask* MakeNoiseFrameComputeNode::CreateComputeTask()
{
	ComputeFramePtr output = GetOutput(_output);

	return ComputeTaskUtils::Make([output]() {
		EComputeFrameType type = output->GetType();
		Size size = output->GetSize();
		switch (type)
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
	});
}

FROX_COMPUTENODE_IMPL(MakeFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeZeroFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeNoiseFrameComputeNode)

} // End frox