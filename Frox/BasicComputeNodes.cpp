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

FROX_COMPUTENODE_IMPL(AddComputeNode)
FROX_COMPUTENODE_IMPL(SubComputeNode)
FROX_COMPUTENODE_IMPL(MulComputeNode)
FROX_COMPUTENODE_IMPL(DivComputeNode)

} // End frox