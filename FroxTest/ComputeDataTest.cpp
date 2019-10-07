#include "Tests.h"
#include "Common.h"
#include "TestProps.h"

#include <MakeFrameComputeNode.h>
#include <FindContoursComputeNode.h>
#include <CenterOfContourComputeNode.h>
#include <RadiusOfContourComputeNode.h>

using namespace frox;

bool findContoursTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto inputFrame = makeCircle(Size{64, 64}, 24);

	auto findContours = flow.CreateNode<FindContoursComputeNode>("FindContours");

	// Set inputs
	inputData.SetFrame("testframe", inputFrame);

	// Connect
	flow.ConnectEntry(flow.CreateEntry("testframe"), findContours);
	flow.ConnectOutput(flow.CreateOutput("contours", EPinValueType::Data), findContours, 0);
	flow.ConnectOutput(flow.CreateOutput("hierarchy", EPinValueType::Data), findContours, 1);

	// Run
	return runFlowBase(
		flow,
		performer,
		inputData,
		ouputData,
		[](FlowData& ouputData) {
			ComputeDataPtr computeData = ouputData.GetData("contours");
			if (!computeData)
			{
				return false;
			}

			ContoursComputeData* contoursComputeData = computeData->As<ContoursComputeData>();
			return contoursComputeData->Contours.size() == 1;
		}
	);
}

bool centerOfContourTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	Size frameSize = Size{ 64, 64 };
	auto inputFrame = makeCircle(frameSize, 24);

	auto findContours = flow.CreateNode<FindContoursComputeNode>("FindContours");
	auto centerOfContours = flow.CreateNode<CenterOfContourComputeNode>("CenterOfContours");

	// Set inputs
	inputData.SetFrame("testframe", inputFrame);

	// Connect
	flow.ConnectEntry(flow.CreateEntry("testframe"), findContours);
	flow.ConnectNodes(findContours, centerOfContours);
	flow.ConnectOutput(flow.CreateOutput("out", EPinValueType::Data), centerOfContours);

	// Run
	return runFlowBase(
		flow,
		performer,
		inputData,
		ouputData,
		[frameSize](FlowData& ouputData) {
			ComputeDataPtr computeData = ouputData.GetData("out");
			if (!computeData)
			{
				return false;
			}

			CenterOfContourComputeData* centerOfContourData = computeData->As<CenterOfContourComputeData>();
			return
				centerOfContourData->Points.size() == 1 &&
				centerOfContourData->Points.front() == frox::Point{ int(frameSize.Width / 2), int(frameSize.Height / 2)};
		}
	);
}

bool radiusOfContourTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	Size frameSize = Size{ 64, 64 };
	uint32_t radius = 24;
	auto inputFrame = makeCircle(frameSize, radius);

	auto findContours = flow.CreateNode<FindContoursComputeNode>("FindContours");
	auto radiusOfContours = flow.CreateNode<RadiusOfContourComputeNode>("RadiusOfContours");

	// Set inputs
	inputData.SetFrame("testframe", inputFrame);

	// Connect
	flow.ConnectEntry(flow.CreateEntry("testframe"), findContours);
	flow.ConnectNodes(findContours, radiusOfContours);
	flow.ConnectOutput(flow.CreateOutput("out", EPinValueType::Data), radiusOfContours);

	// Run
	return runFlowBase(
		flow,
		performer,
		inputData,
		ouputData,
		[frameSize, radius](FlowData& ouputData) {
			ComputeDataPtr computeData = ouputData.GetData("out");
			if (!computeData)
			{
				return false;
			}

			RadiusOfContourComputeData* radiusOfContourData = computeData->As<RadiusOfContourComputeData>();
			return
				radiusOfContourData->Values.size() == 1 &&
				float2::Equal(radiusOfContourData->Values.front().Center, float2{ float(frameSize.Width / 2), float(frameSize.Height / 2) }) &&
				utils::Equal(std::round(radiusOfContourData->Values.front().Radius), float(radius));
		}
	);
}

void Tests::ComputeDataTest()
{
	// Test
	using namespace std::placeholders;
	test("FindContours", std::bind(&findContoursTest, _1));
	test("CenterOfContour", std::bind(&centerOfContourTest, _1));
	test("RadiusOfContour", std::bind(&radiusOfContourTest, _1));
}
