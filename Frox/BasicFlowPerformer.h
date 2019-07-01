#pragma once

#include "FlowPerformer.h"
#include "FlowPerformerImpl.h"

namespace frox {

class BasicFlowPerformer : public FlowPerformer
{
	BasicFlowPerformer(IComputeFlowListerner* listerner);

public:
	virtual ~BasicFlowPerformer() override;

	static BasicFlowPerformer* Create(IComputeFlowListerner* listerner = nullptr)
	{
		return new BasicFlowPerformer(listerner);
	}

	virtual void Perform(ComputeFlow* flow /* inputData */) override;
	virtual void Fetch(ComputeFlow* flow /* outputData */) override;
	virtual uint32_t GetNumActiveTasks() const override;

	FlowPerformerImplPtr GetPerformer() const { return _impl; }

private:
	void OnPerformed();

private:
	FlowPerformerImplPtr _impl;
	IComputeFlowListerner* _listerner;
};

} // End frox