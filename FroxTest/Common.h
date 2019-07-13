#pragma once

#include "Shared.h"

#include <Utils.h>
#include <Types.h>

#include <ComputeFlow.h>
#include <FlowPerformer.h>
#include <FlowData.h>

template<typename FunctionT>
void test(const char* name, FunctionT func)
{
	using namespace frox;

	auto gFrox = FroxInstance();
	assert(gFrox != nullptr);

	// Create Flow
	ComputeFlow* flow = gFrox->CreateComputeFlow();
	FlowPerformer* performer = gFrox->CreateFlowPerformer();
	FlowData* inputData = gFrox->CreateFlowData();
	FlowData* ouputData = gFrox->CreateFlowData();

	std::cout << name << ": ";

	bool result = func(*flow, *performer, *inputData, *ouputData);
	if(!result)
	{
		std::cout << ": invalid" << std::endl;
	}
	else
	{
		std::cout << ": success" << std::endl;
	}

	// Destroy Flow
	gFrox->DestroyFlowData(ouputData);
	gFrox->DestroyFlowData(inputData);
	gFrox->DestroyFlowPerformer(performer);
	gFrox->DestroyComputeFlow(flow);
}

template<typename FunctionT>
bool runFlow(frox::ComputeFlow& flow, frox::FlowPerformer& performer, frox::FlowData& inputData, frox::FlowData& ouputData, FunctionT tester)
{
	using namespace frox;

	// Invoke
	performer.Perform(&flow, &inputData);
	performer.Fetch(&flow, &ouputData);

	// Check
	ComputeFramePtr result = ouputData.GetFrame("out");
	if (!result || !result->IsValid())
	{
		return false;
	}

	return tester(result);
}

// Equals
template<typename T>
_inline bool equalValue(T value, T expected)
{
	return value == expected;
}

template<>
_inline bool equalValue<float>(float value, float expected)
{
	return std::abs(value - expected) < std::numeric_limits<float>::epsilon();
}

// Checkers
template<typename T>
bool checkSingle(frox::ComputeFramePtr frame, T expected)
{
	T value = frame->At<T>(0, 0);
	std::cout << value << " expected " << expected;
	return equalValue(value, expected);
}

template<typename T>
bool checkRange(frox::ComputeFramePtr frame, T lo, T hi)
{
	T min = std::numeric_limits<T>::max();
	T max = std::numeric_limits<T>::lowest();
	frox::utils::Foreach<T>(frame, [&min, &max](T value) {
		min = std::min(min, value);
		max = std::max(max, value);
	});

	std::cout
		<< min << "/" << max
		<< " expected [" << lo << ".." << hi << "]";

	return min >= lo && max <= hi;
}

template<typename T>
bool checkSum(frox::ComputeFramePtr frame, T expected)
{
	T sum = T(0);
	frox::utils::Foreach<T>(frame, [&sum](T value) {
		sum += value;
	});

	std::cout
		<< sum
		<< " expected [" << expected << "]";

	return equalValue(sum, expected);
}