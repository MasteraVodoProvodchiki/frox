#include "Tests.h"
#include "Common.h"
#include "TestGenerator.h"

#include <BasicComputeNodes.h>
#include <AvgComputeNode.h>
#include <MakeFrameComputeNode.h>
#include <ConvertToComputeNode.h>
#include <CropComputeNode.h>
#include <ResizeComputeNode.h>
#include <FrameInfoComputeNode.h>

using namespace frox;

// Tests
template<typename T>
bool makeTest(FlowContext context, uint32_t width, uint32_t height, EComputeFrameType type, T value)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

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
auto makeTestUInt8 = std::bind(&makeTest<uint8_t>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_UInt8, 126);
auto makeTestUInt16 = std::bind(&makeTest<uint16_t>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_UInt16, 16000);
auto makeTestUInt32 = std::bind(&makeTest<uint32_t>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_UInt32, 126000);
auto makeTestFloat = std::bind(&makeTest<float>, std::placeholders::_1, 64, 64, EComputeFrameType::ECFT_Float, 0.5f);

bool binaryOperationTest(
	FlowContext context,
	uint32_t width, uint32_t height, EComputeFrameType type,
	OperationComputeNode::EType operationType, Variant v0, Variant v1, uint32_t result
)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes	
	inputData.SetFrame("A", makeFrame(width, height, type, v0));
	inputData.SetFrame("B", makeFrame(width, height, type, v1));
	OperationComputeNode* opNode = nullptr;
	switch (operationType)
	{
	case OperationComputeNode::ET_Add:
		opNode = flow.CreateNode<AddComputeNode>("Add");
		break;
	case OperationComputeNode::ET_Sub:
		opNode = flow.CreateNode<SubComputeNode>("Sub");
		break;
	case OperationComputeNode::ET_Mul:
		opNode = flow.CreateNode<MulComputeNode>("Mul");
		break;
	case OperationComputeNode::ET_Div:
		opNode = flow.CreateNode<DivComputeNode>("Div");
		break;
	default:
		assert(false);
		break;
	}

	// Connect
	auto A = flow.CreateEntry("A");
	auto B = flow.CreateEntry("B");
	flow.ConnectEntry(A, opNode, 0);
	flow.ConnectEntry(B, opNode, 1);
	flow.ConnectOutput(flow.CreateOutput("out"), opNode);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSumOne, std::placeholders::_1, width * height * result)
	);
}

bool simpleTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

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

	// Connect
	auto A = flow.CreateEntry("A");
	auto B = flow.CreateEntry("B");
	auto C = flow.CreateEntry("C");
	flow.ConnectEntry(A, add, 0);
	flow.ConnectEntry(B, add, 1);
	flow.ConnectEntry(C, mul, 1);
	flow.ConnectOutput(flow.CreateOutput("out"), mul);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSingle<float>, std::placeholders::_1, 9.f)
	);
}

bool multiTest(FlowContext context, uint32_t width, uint32_t height)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto avg = flow.CreateNode<AvgComputeNode>("Avg");
	auto add = flow.CreateNode<AddComputeNode>("Add");
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");

	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);
	make->SetValue(1.f);

	// Connect
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
	
	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSingle<float>, std::placeholders::_1, 2.f)
	);
}

bool noiseTest0(FlowContext context, uint32_t width, uint32_t height, EComputeFrameType type)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(type);

	// Connect
	flow.ConnectOutput(flow.CreateOutput("out"), make);

	// RUn
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkRangeAuto, std::placeholders::_1)
	);
}

bool convertToTest0(FlowContext context, uint32_t width, uint32_t height)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);

	auto convertTo = flow.CreateNode<ConvertToComputeNode>("ConvertTo");
	convertTo->SetType(EComputeFrameType::ECFT_UInt16);
	convertTo->SetAlpha(80.0);
	convertTo->SetBeta(20.0);

	// Connect
	flow.ConnectNodes(make, convertTo);
	flow.ConnectOutput(flow.CreateOutput("out"), convertTo);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkRange<uint16_t>, std::placeholders::_1, 20, 100)
	);
}

bool cropTest0(FlowContext context, uint32_t width, uint32_t height, EComputeFrameType type)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	assert(width >= 16 && height >= 16);

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(type);
	make->SetValue(1);

	auto crop = flow.CreateNode<CropComputeNode>("Crop");
	uint32_t cropWidth = width / 2;
	uint32_t cropHeight = height / 2;

	crop->SetOffset(Point{ 4, 4 });
	crop->SetSize(Size{ cropWidth, cropHeight });

	// Connect
	flow.ConnectNodes(make, crop);
	flow.ConnectOutput(flow.CreateOutput("out"), crop);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSumOne, std::placeholders::_1, cropWidth * cropHeight)
	);
}

bool resizeTest0(FlowContext context, uint32_t width, uint32_t height)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	assert(width >= 16 && height >= 16);

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_UInt16);
	make->SetValue(1);

	auto resize = flow.CreateNode<ResizeComputeNode>("Resize");
	uint32_t resizeWidth = width / 2;
	uint32_t resizeHeight = height / 2;

	resize->SetSize(Size{ resizeWidth, resizeHeight });

	// Connect
	flow.ConnectNodes(make, resize);
	flow.ConnectOutput(flow.CreateOutput("out"), resize);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSum<uint16_t, uint32_t>, std::placeholders::_1, resizeWidth * resizeHeight * 1)
	);
}

bool multiChannelsTest0(FlowContext context, uint32_t width, uint32_t height)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

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

bool dynamicInputPropsTest0(FlowContext context, uint32_t width, uint32_t height)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetValue(1);
	
	inputData.SetValue("width", width);
	inputData.SetValue("height", height);

	make->SetType(EComputeFrameType::ECFT_UInt32);

	// Connect
	flow.CreateEntry("width", EPinValueType::Value);
	flow.CreateEntry("height", EPinValueType::Value);

	flow.ConnectEntry("width", make, "width");
	flow.ConnectEntry("height", make, "height");

	flow.ConnectOutput(flow.CreateOutput("out"), make);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSum<uint32_t, uint32_t>, std::placeholders::_1, width * height * 1)
	);
}


bool frameSizeTest0(FlowContext context, uint32_t width, uint32_t height)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

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
	
	// Connect
	flow.ConnectNodes(makeBig, crop);
	flow.ConnectNodes(frameSize, crop, crop->FindInputByName("size"));

	flow.ConnectOutput(flow.CreateOutput("out"), crop);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&checkSum<uint32_t, uint32_t>, std::placeholders::_1, width * height * 1)
	);
}

template<typename FunctionT>
void testEachType(const char* name, FunctionT func)
{
	using namespace std::placeholders;

	// type=1 - Ignore type none
	for (uint8_t typeIndex = 1; typeIndex < ECFT_NumTypes; ++typeIndex)
	{
		EComputeFrameType type = EComputeFrameType(typeIndex);
		test(name, std::bind(func, _1, type));
	}
}

struct EachFrameType
{
	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT f, ContextT context)
	{
		// type=1 - Ignore type none
		for (uint8_t typeIndex = 1; typeIndex < ECFT_NumTypes; ++typeIndex)
		{
			EComputeFrameType type = EComputeFrameType(typeIndex);
			context(name, f(type));
		}

		return true;
	}
};

struct EachOperationComputeNode
{
	std::vector<std::string> _names;
	EachOperationComputeNode()
		: _names({ "Add", "Sub", "Mul", "Div"})
	{}

	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT f, ContextT context)
	{
		bool result = true;
		for (uint8_t typeIndex = 0; typeIndex < OperationComputeNode::ET_NumTypes; ++typeIndex)
		{
			OperationComputeNode::EType type = OperationComputeNode::EType(typeIndex);
			std::string newName = std::string(name) + " - " + _names[typeIndex];
			result &= context(newName.c_str(), f(type));
		}
		
		return result;
	}
};

void Tests::MainTest()
{
	// Test
	using namespace std::placeholders;
	test("Make Uint8", makeTestUInt8);
	test("Make Uint16", makeTestUInt16);
	test("Make Uint32", makeTestUInt32);
	test("Make Float", makeTestFloat);

	// Add Test for each type and multi channels
	// test("Add", func, EachType());

	std::function<bool(FlowContext, EComputeFrameType)> f =
		std::bind(&binaryOperationTest, _1, 64, 64, _2, OperationComputeNode::ET_Sub, 2, 1, 1);
	// auto f = std::bind(&binaryOperationTest, _1, _2, _3, _4, 64, 64, _5, OperationComputeNode::ET_Sub, 2, 1, 1);
	// makeCommonContext(f);
	/*
	auto testTestP = std::bind(&testTest, _1, _2);

	std::function<bool(EComputeFrameType, FlowContext)> testTest3 = std::bind([](EComputeFrameType frameType, FlowContext context) -> bool {
		return true;
	}, _1, _2);
	generationTest("Add", testTestP, EachFrameType(), BasicFlowContext());
	*/
	// Pack _1, _2, _3, _4
	// generationTest(std::bind(&binaryOperationTest, _1, _2, _3, _4, 64, 64, _5, OperationComputeNode::ET_Sub, 2, 1, 1), EachFrameType());

	/*
	std::function<bool(OperationComputeNode::EType, EComputeFrameType, FlowContext)> binaryOperationTester =
		std::bind(&binaryOperationTest, _3, 64, 64, _2, _1, 2, 1, 1);
	generationTest("Op", binaryOperationTester, EachOperationComputeNode(), EachFrameType(), BasicFlowContext());
	*/
	testEachType("Add", std::bind(&binaryOperationTest, _1, 64, 64, _2, OperationComputeNode::ET_Add, 1, 1, 2));
	testEachType("Sub", std::bind(&binaryOperationTest, _1, 64, 64, _2, OperationComputeNode::ET_Sub, 2, 1, 1));
	testEachType("Mul", std::bind(&binaryOperationTest, _1, 64, 64, _2, OperationComputeNode::ET_Mul, 2, 2, 4));
	testEachType("Div", std::bind(&binaryOperationTest, _1, 64, 64, _2, OperationComputeNode::ET_Div, 4, 2, 2));

	test("Simple", std::bind(&simpleTest, _1));
	test("Multi", std::bind(&multiTest, _1, 64, 64));
	testEachType("Noise", std::bind(&noiseTest0, _1, 64, 64, _2));
	test("ConvertTo", std::bind(&convertToTest0, _1, 64, 64));
	testEachType("Crop", std::bind(&cropTest0, _1, 64, 64, _2));
	test("Resize", std::bind(&resizeTest0, _1, 64, 64));
	test("Multi Channels", std::bind(&multiChannelsTest0, _1, 64, 64));

	test("Dynamic Input Props", std::bind(&dynamicInputPropsTest0, _1, 64, 64));
	test("Frame Size", std::bind(&frameSizeTest0, _1, 64, 64));
}
