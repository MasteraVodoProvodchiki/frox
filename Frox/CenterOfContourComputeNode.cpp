#include "CenterOfContourComputeNode.h"
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

FROX_COMPUTEDATA_IMPL(CenterOfContourComputeData)

namespace functions {

#ifndef WITHOUT_OPENCV
void GetCenters(ComputeDataPtr contours, ComputeDataPtr centers)
{
	// Cast
	ContoursComputeData* contoursComputeData = contours->As<ContoursComputeData>();
	assert(contoursComputeData != nullptr);

	CenterOfContourComputeData* centersComputeData = centers->As<CenterOfContourComputeData>();
	assert(centersComputeData != nullptr);

	// Clear previous
	centersComputeData->Points.clear();
	centersComputeData->Points.reserve(contoursComputeData->Contours.size());

	// Calculate moments
	for (auto& contour : contoursComputeData->Contours)
	{
		cv::Moments M = cv::moments(contour);
		Point point = Point{
			int(M.m10 / M.m00),
			int(M.m01 / M.m00)
		};
		centersComputeData->Points.push_back(point);
	}
}
#else
void GetCenters(ComputeDataPtr contours, ComputeDataPtr centers)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(CenterOfContourComputeNode)

CenterOfContourComputeNode::CenterOfContourComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _output("ouput")
{}

CenterOfContourComputeNode::~CenterOfContourComputeNode()
{}

void CenterOfContourComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_output);
}

bool CenterOfContourComputeNode::IsValid() const
{
	return true;
}

ComputeTask* CenterOfContourComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
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
				output.SetValue<CenterOfContourComputeData>(
					[input](ComputeDataPtr output) {
						functions::GetCenters(input, output);
					}
				);
			}
		);
}

} // End frox