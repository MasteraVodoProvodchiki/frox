#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ComputeData.h"
#include "ComputeDataFactory.h"
#include "ExpressionInput.h"

namespace frox {

// Data
class RadiusOfContourComputeData : public ComputeData
{
	FROX_COMPUTEDATA(RadiusOfContourComputeData, "RadiusOfContour")

public:
	struct RadiusData
	{
		float2 Center;
		float Radius;
	};

	RadiusOfContourComputeData(const ComputeDataInitializer& initializer)
		: ComputeData(initializer)
	{}

	std::vector<RadiusData> Values;
};
// End Data


class RadiusOfContourComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(RadiusOfContourComputeNode, "radiusofcontour")
	using Super = ComputeNodeImpl;

public:
	RadiusOfContourComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~RadiusOfContourComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeDataPtr> _input;
	TOutputData<ComputeDataPtr> _output;
};

} // End frox