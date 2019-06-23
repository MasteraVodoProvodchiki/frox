#include <Frox.h>
#include <Utils.h>

#include <ComputeFlow.h>

#include <BasicComputeNodes.h>
#include <AvgComputeNode.h>
#include <MakeFrameComputeNode.h>
#include <ConvertToComputeNode.h>
#include <CropComputeNode.h>

#include <iostream>
#include <conio.h>
#include <assert.h>
#include <vector>
#include <algorithm>

using namespace frox;

Frox* gFrox = nullptr;

template<typename FunctionT>
void test(const char* name, FunctionT func)
{
	assert(gFrox != nullptr);

	// Create Flow
	ComputeFlow* flow = gFrox->CreateComputeFlow();

	std::cout << name << ": ";

	bool result = func(*flow);
	if(!result)
	{
		std::cout << ": invalid" << std::endl;
	}
	else
	{
		std::cout << ": success" << std::endl;
	}

	// Destroy Flow
	gFrox->DestroyComputeFlow(flow);
}

template<typename FunctionT>
bool runFlow(ComputeFlow& flow, FunctionT tester)
{
	// Invoke
	flow.Perform();
	flow.Fetch();

	// Check
	ComputeFramePtr result = flow.GetOutput(0); // mul->GetOutput();
	if (!result || !result->IsValid())
	{
		return false;
	}

	return tester(result);
}

// Equals
template<typename T>
bool equalValue(T value, T expected)
{
	return value == expected;
}

template<>
bool equalValue(float value, float expected)
{
	return std::abs(value - expected) < std::numeric_limits<float>::epsilon();
}

// Checkers
template<typename T>
bool checkSingle(ComputeFramePtr frame, T expected)
{
	T value = frame->At<T>(0, 0);
	std::cout << value << " expected " << expected;
	return equalValue(value, expected);
}

template<typename T>
bool checkRange(ComputeFramePtr frame, T lo, T hi)
{
	T min = std::numeric_limits<T>::max();
	T max = std::numeric_limits<T>::lowest();
	utils::Foreach<T>(frame, [&min, &max](T value) {
		min = std::min(min, value);
		max = std::max(max, value);
	});

	std::cout
		<< min << "/" << max
		<< " expected [" << lo << ".." << hi << "]";

	return min >= lo && max <= hi;
}

template<typename T>
bool checkSum(ComputeFramePtr frame, T expected)
{
	T sum = T(0);
	utils::Foreach<T>(frame, [&sum](T value) {
		sum += value;
	});

	std::cout
		<< sum
		<< " expected [" << expected << "]";

	return equalValue(sum, expected);
}


// Tests
template<typename T>
bool makeTest(ComputeFlow& flow, uint32_t width, uint32_t height, EComputeFrameType type, T value)
{
	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(type);
	make->SetValue(value);
	flow.ConnectOutput(flow.CreateOutput("out"), make);

	return runFlow(
		flow,
		std::bind(&checkRange<T>, std::placeholders::_1, value, value)
	);
}
auto makeTestUInt8 = std::bind(&makeTest<uint8_t>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_UInt8, 126);
auto makeTestUInt16 = std::bind(&makeTest<uint16_t>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_UInt16, 16000);
auto makeTestUInt32 = std::bind(&makeTest<uint32_t>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_UInt32, 126000);
auto makeTestFloat = std::bind(&makeTest<float>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_Float, 0.5f);

bool simpleTest(ComputeFlow& flow)
{
	// Create nodes
	auto add = flow.CreateNode<AddComputeNode>("Add");
	auto mul = flow.CreateNode<MulComputeNode>("Mul");
	flow.ConnectNodes(add, mul, 0);

	// Set inputs
	add->SetInputScalar(0, 1.f);
	add->SetInputScalar(1, 2.f);
	mul->SetInputScalar(1, 3.f);

	flow.ConnectOutput(flow.CreateOutput("out"), mul);

	return runFlow(
		flow,
		std::bind(&checkSingle<float>, std::placeholders::_1, 9.f)
	);
}

bool multiTest(ComputeFlow& flow, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);
	make->SetValue(1.f);

	auto add = flow.CreateNode<AddComputeNode>("Add");
	auto avg = flow.CreateNode<AvgComputeNode>("Avg");
	flow.ConnectNodes(make, add, 0);
	flow.ConnectNodes(make, add, 1);
	flow.ConnectNodes(add, avg, 0);

	flow.ConnectOutput(flow.CreateOutput("out"), avg);
	// Set inputs
	/*
	std::vector<float> values(width * height);
	std::fill(values.begin(), values.end(), 1.f);

	auto A = frox->CreateComputeFrame(Size{ width, height }, EComputeFrameType::ECFT_Float, values.data());
	auto B = frox->CreateComputeFrame(Size{ width, height }, EComputeFrameType::ECFT_Float, values.data());

	add->SetInput(0, A);
	add->SetInput(1, B);
	*/
	
	return runFlow(
		flow,
		std::bind(&checkSingle<float>, std::placeholders::_1, 2.f)
	);
}

bool noiseTest0(ComputeFlow& flow, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);

	flow.ConnectOutput(flow.CreateOutput("out"), make);


	return runFlow(
		flow,
		std::bind(&checkRange<float>, std::placeholders::_1, 0.f, 1.f)
	);
}

bool convertToTest0(ComputeFlow& flow, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);

	auto convertTo = flow.CreateNode<ConvertToComputeNode>("ConvertTo");
	convertTo->SetType(EComputeFrameType::ECFT_UInt16);
	convertTo->SetAlpha(80.0);
	convertTo->SetBeta(20.0);

	flow.ConnectNodes(make, convertTo);
	flow.ConnectOutput(flow.CreateOutput("out"), convertTo);

	return runFlow(
		flow,
		std::bind(&checkRange<uint16_t>, std::placeholders::_1, 20, 100)
	);
}

bool cropTest0(ComputeFlow& flow, uint32_t width, uint32_t height)
{
	assert(width >= 16 && height >= 16);

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_UInt32);
	make->SetValue(1);

	auto crop = flow.CreateNode<CropComputeNode>("Crop");
	int32_t cropWidth = width / 2;
	int32_t cropHeight = height / 2;

	crop->SetRect(Rect{ 4, 4, cropWidth, cropHeight });

	flow.ConnectNodes(make, crop);
	flow.ConnectOutput(flow.CreateOutput("out"), crop);

	return runFlow(
		flow,
		std::bind(&checkSum<uint32_t>, std::placeholders::_1, cropWidth * cropHeight * 1)
	);
}

int main(int argc, char *argv[])
{
	// Init
	gFrox = FroxInit();

	// Test
	using namespace std::placeholders;
	test("Make Uint8", makeTestUInt8);
	test("Make Uint16", makeTestUInt16);
	test("Make Uint32", makeTestUInt32);
	test("Make Float", makeTestFloat);

	test("Simple Test", std::bind(&simpleTest, _1));
	test("Multi Test", std::bind(&multiTest, _1, 64, 64));
	test("Noise Test", std::bind(&noiseTest0, _1, 64, 64));
	test("ConvertTo Test", std::bind(&convertToTest0, _1, 64, 64));
	test("Crop Test", std::bind(&cropTest0, _1, 64, 64));

	// Shutdown
	FroxShutdown(gFrox);

	// Wait
	_getch();
	return 0;
}