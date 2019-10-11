#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

enum class EGaussianBlurBorderType
{
	Constant = 0,
	Replicate = 1,
	Reflect = 2,
	Wrap = 3,
	Reflect101 = 4,
	Transparent = 5,
	Default = 6,
	Isolated = 7
};

class ComputeTask;
class GaussianBlurComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(GaussianBlurComputeNode, "gaussianblur")
	using Super = ComputeNodeImpl;

public:
	GaussianBlurComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~GaussianBlurComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetKernelSize(Size ksize);
	FROX_API void SetSigmaX(double sigmaX);
	FROX_API void SetSigmaY(double sigmaY);
	FROX_API void SetBorderType(EGaussianBlurBorderType borderType);

private:
	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<Size> _ksize;
	TExpressionInput<double> _sigmaX;
	TExpressionInput<double> _sigmaY;
	EGaussianBlurBorderType _borderType;
	TOutputFrame<ComputeFramePtr> _output;
};

} // End frox