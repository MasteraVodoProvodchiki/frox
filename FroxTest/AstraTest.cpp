#include "Tests.h"
#include "Common.h"

#include <Log.h>

#include <DepthSensorModule.h>
#include <AstraModule.h>

#include <functional>

using namespace frox;

bool astraTest0(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Add MOC
	IAstraModule::Get().InitialiseModule();
	IAstraModule::Get().ShutdownModule();

	return true;
}

void Tests::AstraTest()
{
	IDepthSensorModule::Get().InitialiseModule();

	using namespace std::placeholders;
	test("Astra", std::bind(&astraTest0, _1));

	IDepthSensorModule::Get().ShutdownModule();
}
