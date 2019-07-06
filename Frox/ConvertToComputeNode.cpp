#include "ConvertToComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"

#include "Frox.h"
#include "Utils.h"

#include <assert.h>

namespace frox {

namespace utils {

struct ConvertToOperation
{
	double Alpha;
	double Beta;

	ConvertToOperation(double alpha, double beta)
		: Alpha(alpha)
		, Beta(beta)
	{}

	template<typename OutT, typename LeftT>
	OutT SaturateCast (LeftT In) const
	{
		return OutT(In);
	}

	template<typename LeftT, typename RightT>
	bool operator ()(const LeftT& In, RightT& Out) const
	{
		return Out = SaturateCast<RightT>(Alpha * In + Beta);
	}

	template<>
	bool operator ()(const bool& In, bool& Out) const
	{
		return Out = In;
	}
};

} // End utils

namespace functions {

void ConvertTo(ComputeFramePtr input, ComputeFramePtr output, double alpha, double beta)
{
	utils::Foreach(input, output, utils::ConvertToOperation(alpha, beta));
}

} // End functions

FROX_COMPUTENODE_IMPL(ConvertToComputeNode)

ConvertToComputeNode::ConvertToComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _type(EComputeFrameType::ECFT_UInt8)
	, _alpha("alpha", 1.0)
	, _beta("beta", 0.0)
	, _output("output")
{}

ConvertToComputeNode::~ConvertToComputeNode()
{}

void ConvertToComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
}

bool ConvertToComputeNode::IsValid() const
{
	return _type != EComputeFrameType::ECFT_None;
}

ComputeTask* ConvertToComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	auto input = _input.GetValue(inputData);
	auto alpha = _alpha.GetValue(inputData);
	auto beta = _beta.GetValue(inputData);
	auto type = _type;

	auto output = _output.GetValue(outputData);
	
	return
		ComputeTaskHelper::UnPackProps(input, type, alpha, beta)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, EComputeFrameType type, double alpha, double beta) {
				return input != nullptr && input->IsValid() && type != EComputeFrameType::ECFT_None;
			},
			[output](ComputeFramePtr input, EComputeFrameType type, double alpha, double beta) {
				output.SetValue(
					input->GetSize(),
					ComputeFrameType{ type, input->GetChannels() },
					[input, alpha, beta](ComputeFramePtr output) {
						functions::ConvertTo(input, output, alpha, beta);
					}
				);
			}
		);
}

void ConvertToComputeNode::SetType(EComputeFrameType type)
{
	_type = type;
}

void ConvertToComputeNode::SetAlpha(double alpha)
{
	_alpha = alpha;
}

void ConvertToComputeNode::SetBeta(double beta)
{
	_beta = beta;
}

} // End frox