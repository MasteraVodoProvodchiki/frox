#include "Tests.h"
#include "Common.h"
#include "TestProps.h"

#include <MakeFrameComputeNode.h>
#include <FindContoursComputeNode.h>
#include <CenterOfContourComputeNode.h>

using namespace frox;

ComputeFramePtr makeCircle(Size size)
{
	auto gFrox = FroxInstance();
	assert(gFrox != nullptr);

	// TODO. std::vector<bool> doesn't contain method data
	uint8_t* values = new uint8_t[size.Width * size.Height];
	float radius = float(size.Width - size.Width / 4) * 0.5f;
	for (uint32_t x = 0; x < size.Width; ++x)
	{
		for (uint32_t y = 0; y < size.Height; ++y)
		{
			float d = std::sqrtf(
				std::powf(int(x) - int(size.Width / 2), 2.f) +
				std::powf(int(y) - int(size.Height / 2), 2.f)
			);
			values[y * size.Width + x] = d < radius ? 255 : 0;
		}
	}


	auto frame = gFrox->CreateComputeFrame(size, EComputeFrameType::ECFT_UInt8, values);

	delete[] values;

	return frame;
}

bool findContoursTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	auto inputFrame = makeCircle(Size{64, 64});

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

bool centerOfContoursTest(FlowContext context)
{
	ComputeFlow& flow = context.Flow;
	FlowPerformer& performer = context.Performer;
	FlowData& inputData = context.InputData;
	FlowData& ouputData = context.OuputData;

	// Create nodes
	Size frameSize = Size{ 64, 64 };
	auto inputFrame = makeCircle(frameSize);

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

void Tests::ComputeDataTest()
{
	// Test
	using namespace std::placeholders;
	test("FindContours", std::bind(&findContoursTest, _1));
	test("FindContours", std::bind(&centerOfContoursTest, _1));
}
