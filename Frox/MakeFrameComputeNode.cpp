#include "MakeFrameComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

MakeFrameComputeNode::MakeFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _width(1)
	, _height(1)
	, _type(EComputeFrameType::ECFT_None)
{}

MakeFrameComputeNode::~MakeFrameComputeNode()
{}

void MakeFrameComputeNode::AllocateDefaultPins()
{
	_output = CreateOutput("output");
}

void MakeFrameComputeNode::OnPostInit()
{
	if (_width > 0 && _height > 0 && _type != EComputeFrameType::ECFT_None && _value.IsValid())
	{
		ComputeFramePtr output = FroxInstance()->CreateComputeFrame(Size{ _width, _height }, _type);
		SetOutput(_output, output);
	}
}

bool MakeFrameComputeNode::IsValid() const
{
	return
		_width > 0 && _height > 0 &&
		_value.IsValid() &&
		_type != EComputeFrameType::ECFT_None &&
		GetOutput(_output) != nullptr;
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

void MakeFrameComputeNode::SetWidth(uint32_t width)
{
	_width = width;
}

void MakeFrameComputeNode::SetHeight(uint32_t height)
{
	_height = height;
}

void MakeFrameComputeNode::SetValue(Variant value)
{
	_value = value;
}

void MakeFrameComputeNode::SetType(EComputeFrameType type)
{
	_type = type;
}

MakeZeroFrameComputeNode::MakeZeroFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{
	SetType(EComputeFrameType::ECFT_Float);
	SetValue(0.f);
}

FROX_COMPUTENODE_IMPL(MakeFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeZeroFrameComputeNode)

} // End frox