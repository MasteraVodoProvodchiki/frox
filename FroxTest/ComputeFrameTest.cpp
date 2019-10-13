#include "Tests.h"
#include "Common.h"
#include "TestProps.h"

using namespace frox;

bool loadFrameTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	auto frame = FroxInstance()->LoadComputeFrame("TestFrame.frame", EFrameLoaderType::Gamex);
	bool bSuccess = frame != nullptr;

	FroxInstance()->DestroyComputeFrame(frame);	
	return bSuccess;
}

void Tests::ComputeFrameTests()
{
	// Test
	using namespace std::placeholders;
	test("LoadFrame", std::bind(&loadFrameTest, _1));
}
