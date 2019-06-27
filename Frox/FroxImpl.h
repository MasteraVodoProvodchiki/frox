#pragma once

#include "Frox.h"
#include <vector>


namespace frox {

class FroxImpl : public Frox
{
public:
	FroxImpl(const FroxDesc& desc);
	virtual ~FroxImpl() override;

	// Frox overrides
	virtual ComputeFlow* CreateComputeFlow(IComputeFlowListerner* listerner) override;
	virtual void DestroyComputeFlow(ComputeFlow* computeFlow) override;
	virtual ComputeFramePtr CreateComputeFrame(Size size = Size{ 0, 0 }, ComputeFrameType type = ComputeFrameType{ ECFT_None, 0 }, const void* data = nullptr) override;
	virtual void DestroyComputeFrame(ComputeFramePtr computeFlow) override;

private:
	std::vector<ComputeFlow*> _flows;
};

} // End frox