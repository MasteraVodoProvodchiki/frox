#include "FindContoursComputeNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"
#include "Frox.h"
#include "Utils.h"

#ifndef WITHOUT_OPENCV
#include "OpenCV.h"
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>
#include <algorithm>

namespace frox {

FROX_COMPUTEDATA_IMPL(ContoursComputeData)
FROX_COMPUTEDATA_IMPL(HierarchyComputeData)

namespace functions {

#ifndef WITHOUT_OPENCV
void FindContours(ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy)
{
	assert(input->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	cv::Mat inputMat = cvInput->GetMat();

	ContoursComputeData* contoursComputeData = contours->As<ContoursComputeData>();
	assert(contoursComputeData != nullptr);

	HierarchyComputeData* hierarchyComputeData = hierarchy->As<HierarchyComputeData>();
	assert(hierarchyComputeData != nullptr);

	cv::findContours(inputMat, contoursComputeData->Contours, hierarchyComputeData->Hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
}
#else
void FindContours(ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(FindContoursComputeNode)

FindContoursComputeNode::FindContoursComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _contours("contours")
	, _hierarchy("hierarchy")
{}

FindContoursComputeNode::~FindContoursComputeNode()
{}

void FindContoursComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterOutput(&_contours);
	RegisterOutput(&_hierarchy);
}

bool FindContoursComputeNode::IsValid() const
{
	return true;
}

ComputeTask* FindContoursComputeNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto contours = _contours.GetValue(inputData);
	auto hierarchy = _hierarchy.GetValue(outputData);

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input) {
				return input != nullptr && input->IsValid();
			},
			[contours, hierarchy](ComputeFramePtr input) {
				// TODO. Add `compose` and fix callback hell
				contours.SetValue<ContoursComputeData>(
					[input, hierarchy](ComputeDataPtr contours) {			
						hierarchy.SetValue<HierarchyComputeData>(
							[input, contours](ComputeDataPtr hierarchy) {
								functions::FindContours(input, contours, hierarchy);
							}
						);
					}
				);
			}
		);
}

} // End frox