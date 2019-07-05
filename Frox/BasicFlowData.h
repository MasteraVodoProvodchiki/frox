#pragma once

#include "FlowData.h"
#include "FlowDataImpl.h"

namespace frox {

class BasicFlowData : public FlowData
{
	BasicFlowData();

public:
	virtual ~BasicFlowData() override;

	static BasicFlowData* Create()
	{
		return new BasicFlowData();
	}

	// FlowData overrides
	virtual void SetFrame(const char* name, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetFrame(const char* name) const override;

public:
	FlowDataImplPtr GetData() const { return _impl; }

private:
	FlowDataImplPtr _impl;
};

} // End frox