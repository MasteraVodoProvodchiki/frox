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

	virtual void SetFrame(const char* name, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetFrame(const char* name) const override;

private:
	std::map<std::string, ComputeFramePtr> _frames;
};

} // End frox