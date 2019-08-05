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
	virtual FlowData* CreateFlowData() override;
	virtual void DestroyFlowData(FlowData* flowData) override;
	virtual FlowPerformer* CreateFlowPerformer(IComputeFlowListerner* listerner = nullptr) override;
	virtual void DestroyFlowPerformer(FlowPerformer* performer) override;
	virtual ComputeFramePtr CreateComputeFrame(Size size = Size{ 0, 0 }, ComputeFrameType type = ComputeFrameType{ ECFT_None, 0 }, const void* data = nullptr) override;
	virtual void DestroyComputeFrame(ComputeFramePtr computeFlow) override;
	virtual ComputeDataPtr CreateComputeData(const char* type) override;

private:
	std::vector<ComputeFlow*> _flows;
	std::vector<FlowData*> _datas;
	std::vector<FlowPerformer*> _performers;
};

} // End frox