#include "ShowFrameNode.h"
#include "ComputeTask.h"
#include "ComputeTaskHelper.h"

#include "Frox.h"
#include "Utils.h"
#include "FindContoursComputeNode.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#endif

#include <assert.h>

namespace frox {

namespace functions {

#ifndef WITHOUT_OPENCV
void ShowFrame(const char* name, ComputeFramePtr input)
{
	assert(input->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());

	cv::Mat inputMat = cvInput->GetMat();

	/// Show in a window
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
	cv::imshow(name, inputMat);
	cv::waitKey(1);
}

void ShowFrameWithContours(const char* name, ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy)
{
	assert(input->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(input.get());
	ContoursComputeData* contoursComputeData = contours->As<ContoursComputeData>();
	HierarchyComputeData* hierarchyComputeData = hierarchy->As<HierarchyComputeData>();
	assert(contoursComputeData != nullptr && hierarchyComputeData != nullptr);

	cv::Mat inputMat = cvInput->GetMat();
	const std::vector<std::vector<frox::Point>>& contoursArray = contoursComputeData->Contours;
	const std::vector<frox::uint4>& hierarchyArray = hierarchyComputeData->Hierarchy;

	static_assert(sizeof(cv::Point) == sizeof(frox::Point), "cv::Point != frox::Point");
	const std::vector<std::vector<cv::Point>>& contoursCV = *reinterpret_cast<const std::vector<std::vector<cv::Point>>*>(&contoursArray);

	static_assert(sizeof(cv::Vec4i) == sizeof(frox::uint4), "cv::Vec4i != frox::uint4");
	const std::vector<cv::Vec4i>& hierarchyCV = *reinterpret_cast<const std::vector<cv::Vec4i>*>(&hierarchyArray);

	cv::RNG rng(12345);

	/// Draw contours
	cv::Mat drawing = cv::Mat::zeros(inputMat.size(), CV_8UC3);
	for (int i = 0; i < contoursCV.size(); i++)
	{
		const cv::Vec4i& transitions = hierarchyCV[i];
		cv::Scalar color = transitions[3] == -1 ?
			cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)) :
			cv::Scalar(127, 127, 127);
		drawContours(drawing, contoursCV, i, color, 2, 8, hierarchyCV, 0, cv::Point());
	}

	/// Show in a window
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
	cv::imshow(name, drawing);
	cv::waitKey(1);
}
#else
void ShowFrame(const char* name, ComputeFramePtr input)
{
	assert(false);
}

void ShowFrameWithContours(const char* name, ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarch)
{
	assert(false);
}
#endif // WITHOUT_OPENCV

} // End functions

FROX_COMPUTENODE_IMPL(ShowFrameNode)
FROX_COMPUTENODE_IMPL(ShowFrameWithContoursNode)

void ShowFrameBaseNode::SetName(const std::string& name)
{
	_name = name;
}

//
ShowFrameNode::ShowFrameNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _show("show", true)
{
	SetName("ShowFrameNode");
}

ShowFrameNode::~ShowFrameNode()
{}

void ShowFrameNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_show);
}

bool ShowFrameNode::IsValid() const
{
	return true;
}

ComputeTask* ShowFrameNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto show = _show.GetValue(inputData);
	auto self = getptr();

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, show)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, bool show) {
				return input != nullptr && input->IsValid();
			},
			[self](ComputeFramePtr input, bool show) {
				if (show)
				{
					functions::ShowFrame(self->GetName(), input);
				}
			}
		);
}

//
ShowFrameWithContoursNode::ShowFrameWithContoursNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input("input")
	, _contours("contours")
	, _hierarchy("hierarchy")
	, _show("show", true)
{
	SetName("ShowFrameWithContoursNode");
}

ShowFrameWithContoursNode::~ShowFrameWithContoursNode()
{}

void ShowFrameWithContoursNode::AllocateDefaultPins()
{
	RegisterInput(&_input);
	RegisterInput(&_contours);
	RegisterInput(&_hierarchy);
	RegisterInput(&_show);
}

bool ShowFrameWithContoursNode::IsValid() const
{
	return true;
}

ComputeTask* ShowFrameWithContoursNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// Prepare inputs/output
	auto input = _input.GetValue(inputData);
	auto contours = _contours.GetValue(inputData);
	auto hierarchy = _hierarchy.GetValue(inputData);
	auto show = _show.GetValue(inputData);
	auto self = getptr();

	// Make task
	return
		ComputeTaskHelper::UnPackProps(input, contours, hierarchy, show)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy, bool show) {
				return input != nullptr && input->IsValid();
			},
			[self](ComputeFramePtr input, ComputeDataPtr contours, ComputeDataPtr hierarchy, bool show) {
				if (show)
				{
					functions::ShowFrameWithContours(self->GetName(), input, contours, hierarchy);
				}
			}
		);
}

} // End frox