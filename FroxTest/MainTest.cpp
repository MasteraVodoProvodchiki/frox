#include "Tests.h"
#include "Common.h"

#include <BasicComputeNodes.h>
#include <AvgComputeNode.h>
#include <MakeFrameComputeNode.h>
#include <ConvertToComputeNode.h>
#include <CropComputeNode.h>
#include <FrameInfoComputeNode.h>

using namespace frox;

// Tests
template<typename T>
bool makeTest(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height, EComputeFrameType type, T value)
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
		performer,
		inputData,
		ouputData,
		std::bind(&checkRange<T>, std::placeholders::_1, value, value)
	);
}
auto makeTestUInt8 = std::bind(&makeTest<uint8_t>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 64, 64, EComputeFrameType::ECFT_UInt8, 126);
auto makeTestUInt16 = std::bind(&makeTest<uint16_t>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 64, 64, EComputeFrameType::ECFT_UInt16, 16000);
auto makeTestUInt32 = std::bind(&makeTest<uint32_t>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 64, 64, EComputeFrameType::ECFT_UInt32, 126000);
auto makeTestFloat = std::bind(&makeTest<float>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 64, 64, EComputeFrameType::ECFT_Float, 0.5f);

bool simpleTest(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData)
{
	// Create nodes
	auto add = flow.CreateNode<AddComputeNode>("Add");
	auto mul = flow.CreateNode<MulComputeNode>("Mul");
	flow.ConnectNodes(add, mul, 0);

	// Set inputs
	// add->SetInputScalar(0, 1.f);
	// add->SetInputScalar(1, 2.f);
	// mul->SetInputScalar(1, 3.f);
	inputData.SetInputScalar("A", 1.f);
	inputData.SetInputScalar("B", 2.f);
	inputData.SetInputScalar("C", 3.f);

	auto A = flow.CreateEntry("A");
	auto B = flow.CreateEntry("B");
	auto C = flow.CreateEntry("C");
	flow.ConnectEntry(A, add, 0);
	flow.ConnectEntry(B, add, 1);
	flow.ConnectEntry(C, mul, 1);
	flow.ConnectOutput(flow.CreateOutput("out"), mul);

	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSingle<float>, std::placeholders::_1, 9.f)
	);
}

bool multiTest(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
{
	auto avg = flow.CreateNode<AvgComputeNode>("Avg");
	auto add = flow.CreateNode<AddComputeNode>("Add");
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	// Create nodes
	
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);
	make->SetValue(1.f);

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
		performer,
		inputData,
		ouputData,
		std::bind(&checkSingle<float>, std::placeholders::_1, 2.f)
	);
}

bool noiseTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);

	flow.ConnectOutput(flow.CreateOutput("out"), make);


	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkRange<float>, std::placeholders::_1, 0.f, 1.f)
	);
}

bool convertToTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
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
		performer,
		inputData,
		ouputData,
		std::bind(&checkRange<uint16_t>, std::placeholders::_1, 20, 100)
	);
}

bool cropTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
{
	assert(width >= 16 && height >= 16);

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_UInt32);
	make->SetValue(1);

	auto crop = flow.CreateNode<CropComputeNode>("Crop");
	uint32_t cropWidth = width / 2;
	uint32_t cropHeight = height / 2;

	crop->SetOffset(Point{ 4, 4 });
	crop->SetSize(Size{ cropWidth, cropHeight });

	flow.ConnectNodes(make, crop);
	flow.ConnectOutput(flow.CreateOutput("out"), crop);

	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSum<uint32_t>, std::placeholders::_1, cropWidth * cropHeight * 1)
	);
}

bool multiChannelsTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);
	make->SetChannels(4);

	flow.ConnectOutput(flow.CreateOutput("out"), make);

	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkRange<float4>, std::placeholders::_1, float4(0.f), float4(1.f))
	);
}

bool dynamicInputPropsTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetValue(1);
	
	inputData.SetValue("width", width);
	inputData.SetValue("height", height);

	make->SetType(EComputeFrameType::ECFT_UInt32);

	flow.CreateEntry("width", EPinValueType::Value);
	flow.CreateEntry("height", EPinValueType::Value);

	flow.ConnectEntry("width", make, "width");
	flow.ConnectEntry("height", make, "height");

	flow.ConnectOutput(flow.CreateOutput("out"), make);

	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSum<uint32_t>, std::placeholders::_1, width * height * 1)
	);
}


bool frameSizeTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData, uint32_t width, uint32_t height)
{
	// Create nodes
	auto make0 = flow.CreateNode<MakeFrameComputeNode>("Make");
	make0->SetValue(1);
	make0->SetWidth(width);
	make0->SetHeight(height);
	make0->SetType(EComputeFrameType::ECFT_UInt8);

	auto frameSize = flow.CreateNode<FrameSizeComputeNode>("FrameSize");
	flow.ConnectNodes(make0, frameSize);


	auto makeBig = flow.CreateNode<MakeFrameComputeNode>("Make");
	makeBig->SetValue(1);
	makeBig->SetWidth(width * 2);
	makeBig->SetHeight(height * 2);
	makeBig->SetType(EComputeFrameType::ECFT_UInt32);

	auto crop = flow.CreateNode<CropComputeNode>("Crop");
	
	flow.ConnectNodes(makeBig, crop);
	flow.ConnectNodes(frameSize, crop, crop->FindInputByName("size"));

	flow.ConnectOutput(flow.CreateOutput("out"), crop);

	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSum<uint32_t>, std::placeholders::_1, width * height * 1)
	);
}


void Tests::MainTest()
{
	// Test
	using namespace std::placeholders;
	test("Make Uint8", makeTestUInt8);
	test("Make Uint16", makeTestUInt16);
	test("Make Uint32", makeTestUInt32);
	test("Make Float", makeTestFloat);

	test("Simple", std::bind(&simpleTest, _1, _2, _3, _4));
	test("Multi", std::bind(&multiTest, _1, _2, _3, _4, 64, 64));
	test("Noise", std::bind(&noiseTest0, _1, _2, _3, _4, 64, 64));
	test("ConvertTo", std::bind(&convertToTest0, _1, _2, _3, _4, 64, 64));
	test("Crop", std::bind(&cropTest0, _1, _2, _3, _4, 64, 64));
	test("Multi Channels", std::bind(&multiChannelsTest0, _1, _2, _3, _4, 64, 64));

	test("Dynamic Input Props", std::bind(&dynamicInputPropsTest0, _1, _2, _3, _4, 64, 64));
	test("Frame Size", std::bind(&frameSizeTest0, _1, _2, _3, _4, 64, 64));
}
