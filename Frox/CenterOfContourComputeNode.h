#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ComputeData.h"
#include "ComputeDataFactory.h"
#include "ExpressionInput.h"

namespace frox {

// Data
class CenterOfContourComputeData : public ComputeData
{
	FROX_COMPUTEDATA(CenterOfContourComputeData, "CenterOfContour")

public:
	CenterOfContourComputeData(const ComputeDataInitializer& initializer)
		: ComputeData(initializer)
	{}

	std::vector<Point> Points;
};
// End Data


class CenterOfContourComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(CenterOfContourComputeNode, "centerofcontour")
	using Super = ComputeNodeImpl;

public:
	CenterOfContourComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~CenterOfContourComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeDataPtr> _input;
	TOutputData<ComputeDataPtr> _output;
};

} // End frox