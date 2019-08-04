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
int FindContoursModeToCvMode(EFindContoursMode mode)
{
	switch (mode)
	{
	case EFindContoursMode::External:
		return cv::RETR_EXTERNAL;
	case EFindContoursMode::List:
		return cv::RETR_LIST;
	case EFindContoursMode::CComp:
		return cv::RETR_CCOMP;
	case EFindContoursMode::Tree:
		return cv::RETR_TREE;
	case EFindContoursMode::FloodFill:
		return cv::RETR_FLOODFILL;
	default:
		assert(false);
	}
}

void FindContours(ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy, EFindContoursMode mode)
{
	assert(input->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	cv::Mat inputMat = cvInput->GetMat();

	ContoursComputeData* contoursComputeData = contours->As<ContoursComputeData>();
	assert(contoursComputeData != nullptr);

	HierarchyComputeData* hierarchyComputeData = hierarchy->As<HierarchyComputeData>();
	assert(hierarchyComputeData != nullptr);

	cv::findContours(
		inputMat,
		contoursComputeData->Contours,
		hierarchyComputeData->Hierarchy,
		FindContoursModeToCvMode(mode),
		cv::CHAIN_APPROX_SIMPLE,
		cv::Point(0, 0)
	);
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
	, _mode(EFindContoursMode::External)
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
	auto mode = _mode;
	auto contours = _contours.GetValue(inputData);
	auto hierarchy = _hierarchy.GetValue(outputData);

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, mode)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, EFindContoursMode mode) {
				return input != nullptr && input->IsValid();
			},
			[contours, hierarchy](ComputeFramePtr input, EFindContoursMode mode) {
				// TODO. Add `compose` and fix callback hell
				contours.SetValue<ContoursComputeData>(
					[input, mode, hierarchy](ComputeDataPtr contours) {
						hierarchy.SetValue<HierarchyComputeData>(
							[input, mode, contours](ComputeDataPtr hierarchy) {
								functions::FindContours(input, contours, hierarchy, mode);
							}
						);
					}
				);
			}
		);
}

void FindContoursComputeNode::SetMode(EFindContoursMode mode)
{
	_mode = mode;
}

} // End frox