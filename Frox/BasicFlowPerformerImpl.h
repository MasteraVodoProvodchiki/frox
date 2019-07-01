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

	virtual void Perform(ComputeFlowImplPtr flow) override;
	virtual void Fetch(ComputeFlowImplPtr flow) override;
	virtual uint32_t GetNumActiveTasks() const override;

	void SetOnPerformedCallback(std::function<void()> onPerformed);
	void ClearOnPerformedCallback();

private:
	void OnTaskCompleted(ComputeTask* task);
	void Performed();

private:
	ComputeFlowImplPtr _flow;

	mutable std::mutex _tasksMutex;
	std::vector<ComputeTaskPtr> _tasks;

	mutable std::mutex _onPerformedMutex;
	std::function<void()> _onPerformed;
};

} // End frox