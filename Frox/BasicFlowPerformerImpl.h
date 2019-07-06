#pragma once

#include "FlowPerformerImpl.h"
#include "ComputeFlowImpl.h"
#include "ComputeTask.h"

#include <mutex>

namespace frox {

class BasicFlowPerformerImpl : public FlowPerformerImpl
{
public:
	BasicFlowPerformerImpl();
	virtual ~BasicFlowPerformerImpl();

	static FlowPerformerImplPtr Create()
	{
		return std::make_shared<BasicFlowPerformerImpl>();
	}

	// FlowPerformerImpl overrides
	virtual void Perform(ComputeFlowImplPtr flow, FlowDataImplPtr inputData) override;
	virtual void Fetch(ComputeFlowImplPtr flow, FlowDataImplPtr outputData) override;
	virtual uint32_t GetNumActiveTasks() const override;

public:
	void SetOnPerformedCallback(std::function<void()> onPerformed);
	void ClearOnPerformedCallback();

private:
	void OnTaskCompleted(ComputeTask* task);
	void Performed();

private:
	FlowDataImplPtr _privateData;

	mutable std::mutex _tasksMutex;
	std::vector<ComputeTaskPtr> _tasks;

	mutable std::mutex _onPerformedMutex;
	std::function<void()> _onPerformed;
};

} // End frox