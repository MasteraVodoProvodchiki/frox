#include "Tests.h"
#include "Common.h"

#include <Log.h>

#include <DepthSensorModule.h>
#include <SensorFrameNode.h>
#include <MOCSensorDeviceManager.h>
#include <RealSenseModule.h>

using namespace frox;

bool sensorFrameTest0(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Add MOC
	auto mocDeviceManager = IDepthSensorModule::Get().CreateDeviceManager<MOCSensorDeviceManager>();
	auto mocDevice = mocDeviceManager->CreateDevice();
	assert(mocDevice);

	// Create nodes
	auto sensorDepthFrame = flow.CreateNode<SensorDepthFrameNode>("SensorDepthFrame");
	sensorDepthFrame->SetSensorSerial(mocDevice->GetSerial());
	flow.ConnectOutput(flow.CreateOutput("out"), sensorDepthFrame);

	// Test
	auto result = runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		[](ComputeFramePtr result) {
			// Check
			return true;
		}
	);

	// Remove MOC
	IDepthSensorModule::Get().UnRegisterDeviceManager(mocDeviceManager.get());
	return result;
}

bool sensorRealSenseTest0(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Add RealSense
	IRealSenseModule::Get().InitialiseModule();
	if (IDepthSensorModule::Get().GetNumDevices() == 0)
	{
		Log::Warning("Depth sensors not found");
		return false;
	}

	// Create nodes
	auto sensorDepthFrame = flow.CreateNode<SensorDepthFrameNode>("SensorDepthFrame");
	flow.ConnectOutput(flow.CreateOutput("out"), sensorDepthFrame);

	// Test
	auto result = runFlow(
		flow,
		performer,
		inputData,
		ouputData,
		[](ComputeFramePtr result) {
			// Check
			return true;
		}
	);

	// Remove RealSense
	IRealSenseModule::Get().ShutdownModule();

	return result;
}

void Tests::DepthSensorTest()
{
	IDepthSensorModule::Get().InitialiseModule();

	using namespace std::placeholders;
	test("SensorFrame", std::bind(&sensorFrameTest0, _1));
	test("RealSense", std::bind(&sensorRealSenseTest0, _1));

	IDepthSensorModule::Get().ShutdownModule();
}
