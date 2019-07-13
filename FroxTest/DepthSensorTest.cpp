#include "Tests.h"
#include "Common.h"

#include <SensorFrameNode.h>

using namespace frox;

bool sensorFrameTest0(ComputeFlow& flow, FlowPerformer& performer, FlowData& inputData, FlowData& ouputData)
{
	// Create nodes
	auto sensorDepthFrame = flow.CreateNode<SensorDepthFrameNode>("SensorDepthFrame");
	flow.ConnectOutput(flow.CreateOutput("out"), sensorDepthFrame);

	return runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		[](ComputeFramePtr result) {
			// Check
			return true;
		}
	);
}

void Tests::DepthSensorTest()
{
	using namespace std::placeholders;
	test("SensorFrameTest", std::bind(&sensorFrameTest0, _1, _2, _3, _4));
}
