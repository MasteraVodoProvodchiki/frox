#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ComputeData.h"
#include "ComputeDataFactory.h"
#include "ExpressionInput.h"

#ifndef WITHOUT_OPENCV
#include "OpenCV.h"
#endif

namespace frox {

// Data
class ContoursComputeData : public ComputeData
{
	FROX_COMPUTEDATA(ContoursComputeData, "FindContoursContours")

public:
	ContoursComputeData(const ComputeDataInitializer& initializer)
		: ComputeData(initializer)
	{}

#ifndef WITHOUT_OPENCV
	std::vector<std::vector<cv::Point>> Contours;
#endif
};

class HierarchyComputeData : public ComputeData
{
	FROX_COMPUTEDATA(HierarchyComputeData, "FindContoursHierarchy")

public:
	HierarchyComputeData(const ComputeDataInitializer& initializer)
		: ComputeData(initializer)
	{}

#ifndef WITHOUT_OPENCV
	std::vector<cv::Vec4i> Hierarchy;
#endif
};
// End Data


class FindContoursComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(FindContoursComputeNode, "findcontours")
	using Super = ComputeNodeImpl;

public:
	FindContoursComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~FindContoursComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeFramePtr> _input;
	// TODO. Add other inputs

	TOutputData<ComputeDataPtr> _contours;
	TOutputData<ComputeDataPtr> _hierarchy;
};

} // End frox