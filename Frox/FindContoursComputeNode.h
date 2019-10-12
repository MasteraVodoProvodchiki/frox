#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "ComputeData.h"
#include "ComputeDataFactory.h"
#include "ExpressionInput.h"

namespace frox {

// Data
class ContoursComputeData : public ComputeData
{
	FROX_COMPUTEDATA(ContoursComputeData, "FindContoursContours")

public:
	ContoursComputeData(const ComputeDataInitializer& initializer)
		: ComputeData(initializer)
	{}

	std::vector<std::vector<Point>> Contours;
};

class HierarchyComputeData : public ComputeData
{
	FROX_COMPUTEDATA(HierarchyComputeData, "FindContoursHierarchy")

public:
	HierarchyComputeData(const ComputeDataInitializer& initializer)
		: ComputeData(initializer)
	{}

	std::vector<uint4> Hierarchy;
};
// End Data


enum class EFindContoursMode
{
	External = 0,
	List = 1,
	CComp = 2,
	Tree = 3,
	FloodFill = 4,
};

enum class EContourApproximationMode
{
	ChainApproxNone = 1,
	ChainApproxSimple = 2,
	ChainApproxTC89_L1 = 3,
	ChainApproxTC89_KCOS = 4
};

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

	FROX_API void SetMode(EFindContoursMode mode);
	FROX_API void SetMethod(EContourApproximationMode method);
	FROX_API void SetOffset(Point offset);

private:
	TExpressionInput<ComputeFramePtr> _input;
	EFindContoursMode _mode;
	EContourApproximationMode _method;
	TExpressionInput<Point> _offset;
	TOutputData<ComputeDataPtr> _contours;
	TOutputData<ComputeDataPtr> _hierarchy;
};

} // End frox