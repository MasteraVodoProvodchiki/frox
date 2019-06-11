#include "BasicComputeNodes.h"
#include "ComputeTask.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

namespace utils {

struct AddOperation
{
	template <typename TypeT>
	TypeT operator ()(TypeT A, TypeT B)
	{
		return A + B;
	}
};

struct SubOperation
{
	template <typename TypeT>
	TypeT operator ()(TypeT A, TypeT B)
	{
		return A - B;
	}
};

struct MulOperation
{
	template <typename TypeT>
	TypeT operator ()(TypeT A, TypeT B)
	{
		return A * B;
	}
};

struct DivOperation
{
	template <typename TypeT>
	TypeT operator ()(TypeT A, TypeT B)
	{
		return A / B;
	}
};


template<typename TypeT, typename CallbackT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, ComputeFramePtr output, CallbackT callback)
{
	Size leftSize = left->GetSize();
	Size rightSize = right->GetSize();
	Size outputSize = output->GetSize();
	Size size = outputSize;

	assert(leftSize == outputSize);
	assert(rightSize == outputSize);

	for (uint32_t row = 0; row < size.Height; ++row)
	{
		const TypeT* leftValues = left->GetRowData<TypeT>(row);
		const TypeT* rightValues = right->GetRowData<TypeT>(row);
		TypeT* outputValues = output->GetRowData<TypeT>(row);
		
		for (uint32_t column = 0; column < size.Width; ++column)
		{
			outputValues[column] = callback(leftValues[column], rightValues[column]);
		}
	}
}

template <typename OperatorT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, ComputeFramePtr output)
{
	EComputeFrameType leftType = left->GetType();
	EComputeFrameType rightType = right->GetType();
	EComputeFrameType outputType = output->GetType();
	EComputeFrameType type = outputType;

	assert(leftType == outputType);
	assert(rightType == outputType);

	switch (type)
	{
	case ECFT_UInt8: {
		Foreach<uint8_t>(left, right, output, OperatorT());
		break;
	}
	case ECFT_UInt16: {
		Foreach<uint16_t>(left, right, output, OperatorT());
		break;
	}
	case ECFT_UInt32: {
		Foreach<uint32_t>(left, right, output, OperatorT());
		break;
	}
	case ECFT_Float: {
		Foreach<float>(left, right, output, OperatorT());
		break;
	}
	default:
		assert(false);
		break;
	}
}

} // End utils

OperationComputeNode::OperationComputeNode(const ComputeNodeInitializer& initializer, EType type)
	: Super(initializer)
	, _type(type)
{}

OperationComputeNode::~OperationComputeNode()
{}

void OperationComputeNode::AllocateDefaultPins()
{
	_left = CreateInput("left");
	_right = CreateInput("right");
	_output = CreateOutput("output");
}

void OperationComputeNode::OnInputChanged(uint32_t inId, ComputeFramePtr frame)
{
	ComputeFramePtr left = GetInput(_left);
	ComputeFramePtr output = GetOutput(_output);
	if (left && !output)
	{
		output = FroxInstance()->CreateComputeFrame(left->GetSize(), left->GetType());
		SetOutput(_output, output);
	}
}

bool OperationComputeNode::IsValid() const
{
	ComputeFramePtr left = GetInput(_left);
	ComputeFramePtr right = GetInput(_right);
	ComputeFramePtr output = GetOutput(_output);

	return
		left != nullptr &&
		right != nullptr &&
		output != nullptr &&
		left->GetType() == right->GetType() &&
		left->GetType() == output->GetType();
		left->GetSize() == right->GetSize() &&
		left->GetSize() == output->GetSize();
}

ComputeTask* OperationComputeNode::CreateComputeTask()
{
	EType type = _type;
	ComputeFramePtr left = GetInput(_left);
	ComputeFramePtr right = GetInput(_right);
	ComputeFramePtr output = GetOutput(_output);

	return ComputeTaskUtils::Make([type, left, right, output]() {
		
		switch (type)
		{
		case OperationComputeNode::ET_Add:
			utils::Foreach<utils::AddOperation>(left, right, output);
			break;
		case OperationComputeNode::ET_Sub:
			utils::Foreach<utils::SubOperation>(left, right, output);
			break;
		case OperationComputeNode::ET_Mul:
			utils::Foreach<utils::MulOperation>(left, right, output);
			break;
		case OperationComputeNode::ET_Div:
			utils::Foreach<utils::DivOperation>(left, right, output);
			break;
		default:
			break;
		}
	});
}

MakeFrameComputeNode::MakeFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _width(1)
	, _height(1)
{}

MakeFrameComputeNode::~MakeFrameComputeNode()
{}

void MakeFrameComputeNode::AllocateDefaultPins()
{
	_output = CreateOutput("output");
}

void MakeFrameComputeNode::OnPostInit()
{
	ComputeFramePtr output = FroxInstance()->CreateComputeFrame(Size{ _width, _height });
	SetOutput(_output, output);
}

bool MakeFrameComputeNode::IsValid() const
{
	return _width > 0 && _height > 0 && GetOutput(_output) != nullptr;
}

ComputeTask* MakeFrameComputeNode::CreateComputeTask()
{
	float value = _value;

	ComputeFramePtr output = GetOutput(_output);

	return ComputeTaskUtils::Make([value, output]() {
		EComputeFrameType type = output->GetType();
		Size size = output->GetSize();
		switch (type)
		{
		case ECFT_UInt8: {
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				uint8_t* values = output->GetRowData<uint8_t>(row);
				memset(values, uint8_t(value), size.Width * sizeof(uint8_t));
			}
			break;
		}
		case ECFT_UInt16: {
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				uint16_t* values = output->GetRowData<uint16_t>(row);
				memset(values, uint16_t(value), size.Width * sizeof(uint16_t));
			}
			break;
		}
		case ECFT_UInt32: {
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				uint32_t* values = output->GetRowData<uint32_t>(row);
				memset(values, uint32_t(value), size.Width * sizeof(uint32_t));
			}
			break;
		}
		case ECFT_Float: {
			for (uint32_t row = 0; row < size.Height; ++row)
			{
				float* values = output->GetRowData<float>(row);
				for (uint32_t column = 0; column < size.Width; ++column)
				{
					float& out = *(values + column);
					out = value;
				}
			}
			break;
		}
		default:
			break;
		}
	});
}

MakeZeroFrameComputeNode::MakeZeroFrameComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{
	SetValue(0.f);
}

FROX_COMPUTENODE_IMPL(AddComputeNode)
FROX_COMPUTENODE_IMPL(SubComputeNode)
FROX_COMPUTENODE_IMPL(MulComputeNode)
FROX_COMPUTENODE_IMPL(DivComputeNode)
FROX_COMPUTENODE_IMPL(MakeFrameComputeNode)
FROX_COMPUTENODE_IMPL(MakeZeroFrameComputeNode)

} // End frox