#pragma once

#include "Shared.h"
#include "ConsoleTextColor.h"

#include <Utils.h>
#include <Types.h>

#include <ComputeFlow.h>
#include <FlowPerformer.h>
#include <FlowData.h>

#include <chrono>

struct FlowContext
{
	frox::ComputeFlow& Flow;
	frox::FlowPerformer& Performer;
	frox::FlowData& InputData;
	frox::FlowData& OuputData;
};

struct BasicFlowContext
{
	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT func, ContextT context)
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

		// Time
		auto startTime = std::chrono::high_resolution_clock::now();

		// Test
		FlowContext flowContext{ *flow, *performer, *inputData, *ouputData };
		bool result = context(name, func(flowContext));

		auto finisTime = std::chrono::high_resolution_clock::now();
		auto timeMks = std::chrono::duration_cast<std::chrono::microseconds>(finisTime - startTime).count();

		if (!result)
		{
			ConsoleTextColor consoleColor(ConsoleTextColor::ForeGroundRed);
			std::cout << ": test failed(" << timeMks << "mks)" << std::endl;
		}
		else
		{
			ConsoleTextColor consoleColor(ConsoleTextColor::ForeGroundGreen);
			std::cout << ": test passed(" << timeMks << "mks)" << std::endl;
		}

		// Destroy Flow
		gFrox->DestroyFlowData(ouputData);
		gFrox->DestroyFlowData(inputData);
		gFrox->DestroyFlowPerformer(performer);
		gFrox->DestroyComputeFlow(flow);

		return result;
	}
};

template<typename FunctionT, typename ...ArgsT>
void test(const char* name, FunctionT func, ArgsT... args)
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

	// Time
	auto startTime = std::chrono::high_resolution_clock::now();

	// Test
	FlowContext context{ *flow, *performer, *inputData, *ouputData };
	bool result = func(context, args...);

	auto finisTime = std::chrono::high_resolution_clock::now();
	auto timeMks = std::chrono::duration_cast<std::chrono::microseconds>(finisTime - startTime).count();

	if(!result)
	{
		ConsoleTextColor consoleColor(ConsoleTextColor::ForeGroundRed);
		std::cout << ": test failed(" << timeMks << "mks)" << std::endl;
	}
	else
	{
		ConsoleTextColor consoleColor(ConsoleTextColor::ForeGroundGreen);
		std::cout << ": test passed(" << timeMks << "mks)" << std::endl;
	}

	// Destroy Flow
	gFrox->DestroyFlowData(ouputData);
	gFrox->DestroyFlowData(inputData);
	gFrox->DestroyFlowPerformer(performer);
	gFrox->DestroyComputeFlow(flow);
}

template<typename FunctionT>
bool runFlowBase(frox::ComputeFlow& flow, frox::FlowPerformer& performer, frox::FlowData& inputData, frox::FlowData& ouputData, FunctionT tester)
{
	// Invoke
	performer.Perform(&flow, &inputData);
	performer.Fetch(&flow, &ouputData);

	return tester(ouputData);
}

template<typename FunctionT>
bool runFlow(frox::ComputeFlow& flow, frox::FlowPerformer& performer, frox::FlowData& inputData, frox::FlowData& ouputData, FunctionT tester)
{
	using namespace frox;

	return runFlowBase(
		flow,
		performer,
		inputData,
		ouputData,
		[tester](frox::FlowData& ouputData) {
			// Check
			ComputeFramePtr result = ouputData.GetFrame("out");
			if (!result || !result->IsValid())
			{
				return false;
			}

			return tester(result);
		}
	);
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

bool checkRangeAuto(frox::ComputeFramePtr frame);

template<typename T, typename SumT>
bool checkSum(frox::ComputeFramePtr frame, SumT expected)
{
	SumT sum = SumT(0);
	frox::utils::Foreach<T>(frame, [&sum](T value) {
		sum += value;
	});

	std::cout
		<< sum
		<< " expected [" << expected << "]";

	return equalValue(sum, expected);
}

template<typename T>
bool countOfValue(frox::ComputeFramePtr frame, T expectedValue, uint32_t expectedCount)
{
	uint32_t sum = 0;
	frox::utils::Foreach<T>(frame, [&sum, expectedValue](T value) {
		sum += equalValue(value, expectedValue) ? 1 : 0;
	});

	std::cout
		<< sum
		<< " expected [" << expectedCount << "]";

	return sum == expectedCount;
}

bool countOfValue(frox::ComputeFramePtr frame, frox::Variant expectedValue, uint32_t expectedCount);

bool checkSumOne(frox::ComputeFramePtr frame, uint32_t nbElements);

frox::ComputeFramePtr makeFrame(frox::Size size, frox::EComputeFrameType type, frox::Variant);
frox::ComputeFramePtr makeCircle(frox::Size size = frox::Size{ 64, 64 }, uint32_t radius = 24);

template<typename T>
frox::ComputeFramePtr makeBox(frox::Size size, frox::Point offset, frox::Size extent, frox::EComputeFrameType type, T in, T out)
{
	auto gFrox = frox::FroxInstance();
	assert(gFrox != nullptr);

	// TODO. std::vector<bool> doesn't contain method data
	T* values = new T[size.Width * size.Height];

	int32_t minX = offset.X - extent.Width;
	int32_t minY = offset.Y - extent.Height;

	int32_t maxX = offset.X + extent.Width;
	int32_t maxY = offset.Y + extent.Height;

	for (uint32_t x = 0; x < size.Width; ++x)
	{
		for (uint32_t y = 0; y < size.Height; ++y)
		{
			values[y * size.Width + x] = (x >= minX && y >= minY && x < maxX && y < maxY) ? in : out;
		}
	}

	auto frame = gFrox->CreateComputeFrame(size, type, values);

	delete[] values;

	return frame;
}

frox::ComputeFramePtr makeBoxByType(frox::Size size, frox::Point offset, frox::Size extent, frox::EComputeFrameType type, frox::Variant in, frox::Variant out);
