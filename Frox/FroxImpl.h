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
	virtual ComputeFlow* CreateComputeFlow() override;
	virtual void DestroyComputeFlow(ComputeFlow* computeFlow) override;
	virtual ComputeFramePtr CreateComputeFrame(Size size = Size{ 0, 0 }, EComputeFrameType type = ECFT_None, void* data = nullptr) override;
	virtual void DestroyComputeFrame(ComputeFramePtr computeFlow) override;

private:
	std::vector<ComputeFlow*> _flows;
};

} // End frox