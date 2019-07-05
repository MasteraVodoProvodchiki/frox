#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "Guid.h"

#include <memory>

namespace frox {

using FlowDataImplPtr = std::shared_ptr<class FlowDataImpl>;
class FlowDataImpl : public std::enable_shared_from_this<FlowDataImpl>
{
public:
	virtual ~FlowDataImpl() {}

	std::shared_ptr<FlowDataImpl> GetPtr()
	{
		return shared_from_this();
	}

	virtual void SetFrame(const char* name, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const char* name) const = 0;

	virtual void SetFrame(const Guid& guid, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const Guid& guid) const = 0;
};

} // End frox