#include "BasicComputeNodes.h"
#include "ComputeTask.h"
#include "Frox.h"

namespace frox {

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
	return
		GetInput(_left) != nullptr &&
		GetInput(_right) != nullptr &&
		GetOutput(_output) != nullptr;
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
			break;
		case OperationComputeNode::ET_Sub:
			break;
		case OperationComputeNode::ET_Mul:
			break;
		case OperationComputeNode::ET_Div:
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