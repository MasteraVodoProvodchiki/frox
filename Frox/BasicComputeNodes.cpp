#include "BasicComputeNodes.h"
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

namespace functions {
#ifndef WITHOUT_OPENCV
void InvokeBinaryOperation(ComputeFramePtr left, ComputeFramePtr right, ComputeFramePtr output, OperationComputeNode::EType type)
{
	assert(left->GetType() == output->GetType());
	assert(right->GetType() == output->GetType());
	assert(left->IsOpencv() && right->IsOpencv() && output->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvLeft = reinterpret_cast<OpenCVComputeFrameImpl*>(left.get());
	OpenCVComputeFrameImpl* cvRight = reinterpret_cast<OpenCVComputeFrameImpl*>(right.get());
	OpenCVComputeFrameImpl* cvOutput = reinterpret_cast<OpenCVComputeFrameImpl*>(output.get());
	cv::Mat leftMat = cvLeft->GetMat();
	cv::Mat rightMat = cvRight->GetMat();
	cv::Mat outputMat = cvOutput->GetMat();

	// operation
	switch (type)
	{
	case OperationComputeNode::ET_Add:
		outputMat = leftMat + rightMat;
		break;
	case OperationComputeNode::ET_Sub:
		outputMat = leftMat - rightMat;
		break;
	case OperationComputeNode::ET_Mul:
		outputMat = leftMat * rightMat;
		break;
	case OperationComputeNode::ET_Div:
		outputMat = leftMat / rightMat;
		break;
	default:
		break;
	}
}
#else
void InvokeBinaryOperation(ComputeFramePtr left, ComputeFramePtr right, ComputeFramePtr output, OperationComputeNode::EType type)
{
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
}
#endif // WITHOUT_OPENCV
} // End functins

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
}

bool OperationComputeNode::IsValid() const
{
	return true;
}

ComputeTask* OperationComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs
	EType type = _type;
	auto left = _left.GetValue(inputData);
	auto right = _right.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	// Make frame
	return ComputeTaskHelper::UnPackProps(left, right)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr left, ComputeFramePtr right) {
				return
					left != nullptr &&
					right != nullptr &&
					left->GetType().Type != EComputeFrameType::ECFT_Bool &&
					left->GetType() == right->GetType() &&
					left->GetSize() == right->GetSize();
			},
			[type, output](ComputeFramePtr left, ComputeFramePtr right) {
				output.SetValue(
					left->GetSize(),
					left->GetType(),
					[type, left, right](ComputeFramePtr output) {
						functions::InvokeBinaryOperation(left, right, output, type);
					}
				);
			}
		);
}

FROX_COMPUTENODE_IMPL(AddComputeNode)
FROX_COMPUTENODE_IMPL(SubComputeNode)
FROX_COMPUTENODE_IMPL(MulComputeNode)
FROX_COMPUTENODE_IMPL(DivComputeNode)

} // End frox