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
	, _left("left")
	, _right("right")
	, _output("output")
{}

OperationComputeNode::~OperationComputeNode()
{}

void OperationComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_left);
	RegisterInput(&_right);
	RegisterOutput(&_output);
	// _left = CreateInput("left");
	// _right = CreateInput("right");
	// _output = CreateOutput("output");
}

/*
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
*/
bool OperationComputeNode::IsValid() const
{
	/*
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
	*/
	return true;
}

ComputeTask* OperationComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Calculate By Expression

	EType type = _type;
	// ComputeFramePtr left = inputData->GetFrame(_left); // GetInput(_left);
	// ComputeFramePtr right = GetInput(_right);
	// ComputeFramePtr output = GetOutput(_output);
	auto left = _left.GetValue(inputData);
	auto right = _right.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	return ComputeTaskUtils::Make([type, left, right, output]() {
		ComputeFramePtr leftFrame = *left;
		ComputeFramePtr rightFrame = *right;
		// Check

		output.SetValue(
			leftFrame->GetSize(),
			leftFrame->GetType(),
			[type, leftFrame, rightFrame](ComputeFramePtr outputFrame) {
				switch (type)
				{
				case OperationComputeNode::ET_Add:
					utils::Foreach(leftFrame, rightFrame, outputFrame, utils::AddOperation());
					break;
				case OperationComputeNode::ET_Sub:
					utils::Foreach(leftFrame, rightFrame, outputFrame, utils::SubOperation());
					break;
				case OperationComputeNode::ET_Mul:
					utils::Foreach(leftFrame, rightFrame, outputFrame, utils::MulOperation());
					break;
				case OperationComputeNode::ET_Div:
					utils::Foreach(leftFrame, rightFrame, outputFrame, utils::DivOperation());
					break;
				default:
					break;
				}
			}
		);
		
	});
}

FROX_COMPUTENODE_IMPL(AddComputeNode)
FROX_COMPUTENODE_IMPL(SubComputeNode)
FROX_COMPUTENODE_IMPL(MulComputeNode)
FROX_COMPUTENODE_IMPL(DivComputeNode)

} // End frox