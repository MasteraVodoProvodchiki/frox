#include "Tests.h"
#include "Common.h"

#ifdef WITH_KINECT2
#include <Log.h>

#include <DepthSensorModule.h>
#include <Kinect2Module.h>

#include <functional>

#pragma comment(lib,"Kinect20.lib")

using namespace frox;

bool kinect2Test0(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Add MOC
	IKinect2Module::Get().InitialiseModule();
	IKinect2Module::Get().ShutdownModule();

	return true;
}

void Tests::Kinect2Test()
{
	IDepthSensorModule::Get().InitialiseModule();

	using namespace std::placeholders;
	test("Kinect2", std::bind(&kinect2Test0, _1));

	IDepthSensorModule::Get().ShutdownModule();
}
#endif // WITH_KINECT2