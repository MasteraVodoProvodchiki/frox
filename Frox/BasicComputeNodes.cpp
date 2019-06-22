#include "BasicComputeNodes.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

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

	template <>
	bool operator ()(bool A, bool B)
	{
		return false;
	}
};

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
			utils::Foreach(left, right, output, utils::AddOperation());
			break;
		case OperationComputeNode::ET_Sub:
			utils::Foreach(left, right, output, utils::SubOperation());
			break;
		case OperationComputeNode::ET_Mul:
			utils::Foreach(left, right, output, utils::MulOperation());
			break;
		case OperationComputeNode::ET_Div:
			utils::Foreach(left, right, output, utils::DivOperation());
			break;
		default:
			break;
		}
	});
}

FROX_COMPUTENODE_IMPL(AddComputeNode)
FROX_COMPUTENODE_IMPL(SubComputeNode)
FROX_COMPUTENODE_IMPL(MulComputeNode)
FROX_COMPUTENODE_IMPL(DivComputeNode)

} // End frox