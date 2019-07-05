#pragma once

#include "FlowDataImpl.h"
#include <map>


namespace frox {

class BasicFlowDataImpl : public FlowDataImpl
{
public:
	BasicFlowDataImpl();
	virtual ~BasicFlowDataImpl();

	static FlowDataImplPtr Create()
	{
		return std::make_shared<BasicFlowDataImpl>();
	}

	// FlowDataImpl overrides
	virtual void SetFrame(const char* name, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetFrame(const char* name) const override;
	virtual void SetFrame(const Guid& guid, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetFrame(const Guid& guid) const override;

private:
	std::map<std::string, ComputeFramePtr> _framesByName;
	std::map<Guid, ComputeFramePtr> _framesByGuid;
};

} // End frox