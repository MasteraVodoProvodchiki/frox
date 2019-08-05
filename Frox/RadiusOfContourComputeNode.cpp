#include "RadiusOfContourComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#include "FindContoursComputeNode.h"

#ifndef WITHOUT_OPENCV
#include "OpenCV.h"
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>
#include <algorithm>

namespace frox {

FROX_COMPUTEDATA_IMPL(RadiusOfContourComputeData)

namespace functions {

#ifndef WITHOUT_OPENCV
void GetRadiuses(ComputeDataPtr contours, ComputeDataPtr centers)
{
	// Cast
	ContoursComputeData* contoursComputeData = contours->As<ContoursComputeData>();
	assert(contoursComputeData != nullptr);

	RadiusOfContourComputeData* radiusOfContourData = centers->As<RadiusOfContourComputeData>();
	assert(radiusOfContourData != nullptr);

	// Clear previous
	radiusOfContourData->Values.clear();
	radiusOfContourData->Values.reserve(contoursComputeData->Contours.size());

	// Calculate moments
	for (auto& contour : contoursComputeData->Contours)
	{
		cv::Point2f center;
		float radius;
		cv::minEnclosingCircle(contour, center, radius);

		radiusOfContourData->Values.push_back(RadiusOfContourComputeData::RadiusData{
			float2{center.x, center.y},
			radius
		});
	}
}
#else
void GetCenters(ComputeDataPtr contours, ComputeDataPtr centers)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(RadiusOfContourComputeNode)

RadiusOfContourComputeNode::RadiusOfContourComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _output("ouput")
{}

RadiusOfContourComputeNode::~RadiusOfContourComputeNode()
{}

void RadiusOfContourComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
}

bool RadiusOfContourComputeNode::IsValid() const
{
	return true;
}

ComputeTask* RadiusOfContourComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto output = _output.GetValue(outputData);

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeDataPtr input) {
				return input != nullptr && input->IsValid();
			},
			[output](ComputeDataPtr input) {
				output.SetValue<RadiusOfContourComputeData>(
					[input](ComputeDataPtr output) {
						functions::GetRadiuses(input, output);
					}
				);
			}
		);
}

} // End frox