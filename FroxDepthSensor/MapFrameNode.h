#pragma once

#include <ComputeNodeImpl.h>
#include <ComputeNodeFactory.h>
#include <ComputeFrame.h>

#include "Shared.h"

namespace frox {

class ComputeTask;
class MapDepthToColorNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(MapDepthToColorNode, "mapDepthToColor")
	using Super = ComputeNodeImpl;

public:
	MapDepthToColorNode(const ComputeNodeInitializer& initializer);
	virtual ~MapDepthToColorNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

protected:
	TExpressionInput<ComputeFramePtr> _depthFrame;
	TExpressionInput<ComputeFramePtr> _colorFrame;
	TOutputFrame<ComputeFramePtr> _output;
};

}
