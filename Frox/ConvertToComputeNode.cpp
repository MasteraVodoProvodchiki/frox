#include "ConvertToComputeNode.h"
#include "ComputeTask.h"
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
	// , _input(0)
	// , _output(0)
	, _input("input")
	, _output("output")
	, _type(EComputeFrameType::ECFT_UInt8)
	, _alpha("alpha", 1.0)
	, _beta("beta", 0.0)
{}

ConvertToComputeNode::~ConvertToComputeNode()
{}

void ConvertToComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
	/*
	_input = CreateInput("input");
	_output = CreateOutput("output");
	*/
}

/*
void ConvertToComputeNode::OnPostInit()
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);
	if (input && !output)
	{
		ComputeFramePtr output = FroxInstance()->CreateComputeFrame(input->GetSize(), _type);
		SetOutput(_output, output);
	}
}
*/

bool ConvertToComputeNode::IsValid() const
{
	/*
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);

	return
		input != nullptr &&
		output != nullptr &&
		input->GetSize() == output->GetSize() &&
		input->IsValid();
	*/
	return true;
}

ComputeTask* ConvertToComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// ComputeFramePtr input = GetInput(_input);
	// ComputeFramePtr output = GetOutput(_output);
	// double alpha = _alpha;
	// double beta = _beta;
	auto input = _input.GetValue(inputData);
	auto alpha = _alpha.GetValue(inputData);
	auto beta = _beta.GetValue(inputData);
	auto output = _output.GetValue(outputData);
	auto type = _type;

	return ComputeTaskUtils::Make([input, type, alpha, beta, output]() {
		ComputeFramePtr inputFrame = *input;
		double alphaValue = *alpha;
		double betaValue = *beta;
		// Check IsValid
		
		output.SetValue(
			inputFrame->GetSize(),
			ComputeFrameType{ type, inputFrame->GetChannels() },
			[inputFrame, alphaValue, betaValue](ComputeFramePtr outputFrame) {
				functions::ConvertTo(inputFrame, outputFrame, alphaValue, betaValue);
			}
		);	
	});
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