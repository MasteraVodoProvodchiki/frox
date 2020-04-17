#pragma once

#include "Common.h"
#include "ComputeFlowImpl.h"
#include "FlowDataImpl.h"

#include <memory>

namespace frox {

using FlowPerformerImplPtr = std::shared_ptr<class FlowPerformerImpl>;
class FlowPerformerImpl : public std::enable_shared_from_this<FlowPerformerImpl>
{
public:
	virtual ~FlowPerformerImpl() {}

	std::shared_ptr<FlowPerformerImpl> GetPtr()
	{
		return shared_from_this();
	}

	virtual void Perform(ComputeFlowImplPtr flow, FlowDataImplPtr inputData) = 0;
	virtual void Fetch(ComputeFlowImplPtr flow, FlowDataImplPtr outputData) = 0;
	virtual uint32_t GetNumActiveTasks() const = 0;

#ifdef FROX_DEBUG
	virtual FlowDataImplPtr GetImmidiatePerformerData() const = 0;
#endif
};

} // End frox