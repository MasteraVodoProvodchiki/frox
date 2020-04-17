#pragma once

#include "DebugFlowData.h"
#include "FlowDataImpl.h"

namespace frox {

class DebugFlowDataImpl : public DebugFlowData
{
public:
	DebugFlowDataImpl(FlowDataImplPtr source) : _source(source) {}

	void SetValue(const Guid& guid, Variant value) override;
	Variant GetValue(const Guid& guid) const override;

	void SetFrame(const Guid& guid, ComputeFramePtr frame) override;
	ComputeFramePtr GetFrame(const Guid& guid) const override;

	void SetData(const Guid& guid, ComputeDataPtr data) override;
	ComputeDataPtr GetData(const Guid& guid) const override;

private:
	FlowDataImplPtr _source;
};

} // End frox
