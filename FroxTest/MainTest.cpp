#include "Tests.h"
#include "Common.h"
#include "TestProps.h"

#include <BasicComputeNodes.h>
#include <AvgComputeNode.h>
#include <MakeFrameComputeNode.h>
#include <ConvertToComputeNode.h>
#include <InRangeComputeNode.h>
#include <CropComputeNode.h>
#include <ResizeComputeNode.h>
#include <FrameInfoComputeNode.h>

using namespace frox;

// Tests
template<typename T>
bool makeTest(FlowContext context, Size size, EComputeFrameType type, T value)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
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
auto makeTestUInt8 = std::bind(&makeTest<uint8_t>, std::placeholders::_1, Size{ 64, 64 }, EComputeFrameType::ECFT_UInt8, 126);
auto makeTestUInt16 = std::bind(&makeTest<uint16_t>, std::placeholders::_1, Size{ 64, 64 }, EComputeFrameType::ECFT_UInt16, 16000);
auto makeTestUInt32 = std::bind(&makeTest<uint32_t>, std::placeholders::_1, Size{ 64, 64 }, EComputeFrameType::ECFT_UInt32, 126000);
auto makeTestFloat = std::bind(&makeTest<float>, std::placeholders::_1, Size{ 64, 64 }, EComputeFrameType::ECFT_Float, 0.5f);

struct BinaryOperationTestData
{
	Variant V0;
	Variant V1;
	uint32_t Result;
};

struct BinaryOperationTestDataProp
{
	OperationComputeNode::EType OperationType;
	BinaryOperationTestDataProp(OperationComputeNode::EType operationType)
		: OperationType(operationType)
	{}

	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT f, ContextT context)
	{
		BinaryOperationTestData data;
		switch (OperationType)
		{
		case OperationComputeNode::ET_Add:
			data.V0 = 1; data.V1 = 1; data.Result = 2;
			break;
		case OperationComputeNode::ET_Sub:
			data.V0 = 2; data.V1 = 1; data.Result = 1;
			break;
		case OperationComputeNode::ET_Mul:
			data.V0 = 2; data.V1 = 2; data.Result = 4;
			break;
		case OperationComputeNode::ET_Div:
			data.V0 = 4; data.V1 = 2; data.Result = 2;
			break;
		default:
			break;
		}

		return context(name, f(data));
	}
};

struct OperationComputeNodeProp
{
	std::vector<std::string> _names;
	OperationComputeNode::EType OperationType;
	OperationComputeNodeProp(OperationComputeNode::EType operationType)
		: _names({ "Add", "Sub", "Mul", "Div" })
		, OperationType(operationType)
	{}

	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT f, ContextT context)
	{
		std::string newName = std::string(name) + " - " + _names[OperationType];
		return context(newName.c_str(), f(OperationType));
	}
};

struct EachOperationComputeNode
{
	std::vector<std::string> _names;
	EachOperationComputeNode()
		: _names({ "Add", "Sub", "Mul", "Div" })
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

bool binaryOperationTest(
	FlowContext context,
	Size size, EComputeFrameType type,
	OperationComputeNode::EType operationType, BinaryOperationTestData data
)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	Variant v0 = data.V0;
	Variant v1 = data.V1;
	uint32_t result = data.Result;

	// Create nodes	
	inputData.SetFrame("A", makeFrame(size, type, v0));
	inputData.SetFrame("B", makeFrame(size, type, v1));
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
		std::bind(&checkSumOne, std::placeholders::_1, size.Width * size.Height * result)
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

bool multiTest(FlowContext context, Size size)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto avg = flow.CreateNode<AvgComputeNode>("Avg");
	auto add = flow.CreateNode<AddComputeNode>("Add");
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");

	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
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

bool noiseTest0(FlowContext context, Size size, EComputeFrameType type)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
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

bool convertToTest0(FlowContext context, Size size)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
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

bool inRangeTest0(FlowContext context, Size size, EComputeFrameType type)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	Point offset = Point{ int32_t(size.Width / 2), int32_t(size.Height / 2) };
	Size boxSize = Size{ 8, 8 };
	auto inputFrame = makeBoxByType(size, offset, boxSize, type, 500, 5);
	

	auto make = flow.CreateNode<ConstFrameComputeNode>("Make");
	make->SetFrame(inputFrame);

	auto inRange = flow.CreateNode<InRangeComputeNode>("InRange");
	inRange->SetLow(10);
	inRange->SetHigh(600);

	// Connect
	flow.ConnectNodes(make, inRange);
	flow.ConnectOutput(flow.CreateOutput("out"), inRange);

	// Run
	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		std::bind(&countOfValue<uint8_t>, std::placeholders::_1, 255, boxSize.Width * 2 * boxSize.Height * 2)
	);
}

bool cropTest0(FlowContext context, Size size, EComputeFrameType type)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	assert(size.Width >= 16 && size.Height >= 16);

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
	make->SetType(type);
	make->SetValue(1);

	auto crop = flow.CreateNode<CropComputeNode>("Crop");
	uint32_t cropWidth = size.Width / 2;
	uint32_t cropHeight = size.Width / 2;

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

bool resizeTest0(FlowContext context, Size size)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	assert(size.Width >= 16 && size.Height >= 16);

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
	make->SetType(EComputeFrameType::ECFT_UInt16);
	make->SetValue(1);

	auto resize = flow.CreateNode<ResizeComputeNode>("Resize");
	uint32_t resizeWidth = size.Width / 2;
	uint32_t resizeHeight = size.Height / 2;

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

bool multiChannelsTest0(FlowContext context, Size size)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeNoiseFrameComputeNode>("Make");
	make->SetWidth(size.Width);
	make->SetHeight(size.Height);
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

bool dynamicInputPropsTest0(FlowContext context, Size size)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make = flow.CreateNode<MakeFrameComputeNode>("Make");
	make->SetValue(1);
	
	inputData.SetValue("width", size.Width);
	inputData.SetValue("height", size.Height);

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
		std::bind(&checkSum<uint32_t, uint32_t>, std::placeholders::_1, size.Width * size.Height * 1)
	);
}


bool frameSizeTest0(FlowContext context, Size size)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto make0 = flow.CreateNode<MakeFrameComputeNode>("Make");
	make0->SetValue(1);
	make0->SetWidth(size.Width);
	make0->SetHeight(size.Height);
	make0->SetType(EComputeFrameType::ECFT_UInt8);

	auto frameSize = flow.CreateNode<FrameSizeComputeNode>("FrameSize");
	flow.ConnectNodes(make0, frameSize);


	auto makeBig = flow.CreateNode<MakeFrameComputeNode>("Make");
	makeBig->SetValue(1);
	makeBig->SetWidth(size.Width * 2);
	makeBig->SetHeight(size.Height * 2);
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
		std::bind(&checkSum<uint32_t, uint32_t>, std::placeholders::_1, size.Width * size.Height * 1)
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

void Tests::MainTest()
{
	// Test
	using namespace std::placeholders;
	test("Make Uint8", makeTestUInt8);
	test("Make Uint16", makeTestUInt16);
	test("Make Uint32", makeTestUInt32);
	test("Make Float", makeTestFloat);


	std::function<bool(OperationComputeNode::EType, EComputeFrameType, BinaryOperationTestData, Size, FlowContext)> binaryOperationTester =
		std::bind(&binaryOperationTest, _5, _4, _2, _1, _3);
	generationTest(
		"Op",
		binaryOperationTester,
		OperationComputeNodeProp(OperationComputeNode::ET_Add),
		EachFrameType().Ignored({ EComputeFrameType::ECFT_Bool }),
		BinaryOperationTestDataProp(OperationComputeNode::ET_Add),
		EachFrameSize(),
		BasicFlowContext()
	);

	generationTest(
		"Op",
		binaryOperationTester,
		OperationComputeNodeProp(OperationComputeNode::ET_Sub),
		EachFrameType().Ignored({ EComputeFrameType::ECFT_Bool }),
		BinaryOperationTestDataProp(OperationComputeNode::ET_Sub),
		EachFrameSize(),
		BasicFlowContext()
	);

	generationTest(
		"Op",
		binaryOperationTester,
		OperationComputeNodeProp(OperationComputeNode::ET_Mul),
		EachFrameType().Ignored({ EComputeFrameType::ECFT_Bool }),
		BinaryOperationTestDataProp(OperationComputeNode::ET_Mul),
		EachFrameSize(),
		BasicFlowContext()
	);

	generationTest(
		"Op",
		binaryOperationTester,
		OperationComputeNodeProp(OperationComputeNode::ET_Div),
		EachFrameType().Ignored({ EComputeFrameType::ECFT_Bool }),
		BinaryOperationTestDataProp(OperationComputeNode::ET_Div),
		EachFrameSize(),
		BasicFlowContext()
	);

	test("Simple", std::bind(&simpleTest, _1));
	test("Multi", std::bind(&multiTest, _1, Size{ 64, 64 }));
	testEachType("Noise", std::bind(&noiseTest0, _1, Size{ 64, 64 }, _2));
	test("ConvertTo", std::bind(&convertToTest0, _1, Size{ 64, 64 }));

	std::function<bool(EComputeFrameType, Size, FlowContext)> inRangeTestTester = std::bind(&inRangeTest0, _3, _2, _1);
	generationTest(
		"InRange",
		inRangeTestTester,
		EachFrameType().Ignored({ EComputeFrameType::ECFT_Bool, EComputeFrameType::ECFT_UInt8 }),
		EachFrameSize({ Size{ 64, 64 }, Size{ 58, 44 } }),
		BasicFlowContext()
	);

	testEachType("Crop", std::bind(&cropTest0, _1, Size{ 64, 64 }, _2));
	test("Resize", std::bind(&resizeTest0, _1, Size{ 64, 64 }));
	test("Multi Channels", std::bind(&multiChannelsTest0, _1, Size{ 64, 64 }));

	test("Dynamic Input Props", std::bind(&dynamicInputPropsTest0, _1, Size{ 64, 64 }));
	test("Frame Size", std::bind(&frameSizeTest0, _1, Size{ 64, 64 }));
}
