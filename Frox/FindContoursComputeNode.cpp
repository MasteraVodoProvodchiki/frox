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

	return cv::RETR_EXTERNAL;
}

int FindContoursMethodToCvMethod(EContourApproximationMode method)
{
	switch (method)
	{
	case EContourApproximationMode::ChainApproxNone:
		return cv::CHAIN_APPROX_NONE;
	case EContourApproximationMode::ChainApproxSimple:
		return cv::CHAIN_APPROX_SIMPLE;
	case EContourApproximationMode::ChainApproxTC89_L1:
		return cv::CHAIN_APPROX_TC89_L1;
	case EContourApproximationMode::ChainApproxTC89_KCOS:
		return cv::CHAIN_APPROX_TC89_KCOS;
	default:
		assert(false);
	}

	return cv::CHAIN_APPROX_SIMPLE;
}

void FindContours(ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy, EFindContoursMode mode, EContourApproximationMode method, frox::Point offset)
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
		FindContoursMethodToCvMethod(method),
		cv::Point(offset.X, offset.Y)
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
	, _method(EContourApproximationMode::ChainApproxSimple)
	, _offset("offset", Point{ 0, 0 })
	, _contours("contours")
	, _hierarchy("hierarchy")
{}

FindContoursComputeNode::~FindContoursComputeNode()
{}

void FindContoursComputeNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_offset);
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
	auto method = _method;
	auto offset = _offset.GetValue(inputData);
	auto contours = _contours.GetValue(inputData);
	auto hierarchy = _hierarchy.GetValue(outputData);

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, mode, method, offset)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, EFindContoursMode mode, EContourApproximationMode method, Point offset) {
				return input != nullptr && input->IsValid();
			},
			[contours, hierarchy](ComputeFramePtr input, EFindContoursMode mode, EContourApproximationMode method, Point offset) {
				// TODO. Add `compose` and fix callback hell
				contours.SetValue<ContoursComputeData>(
					[input, mode, method, offset, hierarchy](ComputeDataPtr contours) {
						hierarchy.SetValue<HierarchyComputeData>(
							[input, mode, method, offset, contours](ComputeDataPtr hierarchy) {
								functions::FindContours(input, contours, hierarchy, mode, method, offset);
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

void FindContoursComputeNode::SetMethod(EContourApproximationMode method)
{
	_method = method;
}

void FindContoursComputeNode::SetOffset(Point offset)
{
	_offset = offset;
}

} // End frox