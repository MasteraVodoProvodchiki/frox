#include "BasicFlowPerformerImpl.h"
#include "Log.h"
#include "ComputeThread.h"
#include "ComputeNodeImpl.h"

#include <assert.h>
#include <algorithm>

namespace frox {

BasicFlowPerformerImpl::BasicFlowPerformerImpl()
{}

BasicFlowPerformerImpl::~BasicFlowPerformerImpl()
{}

void BasicFlowPerformerImpl::Perform(ComputeFlowImplPtr flow)
{
	assert(flow);

	flow->Prepare();

	// Update entries
	const ComputeFlowEntry* entries = nullptr;
	uint32_t nbEntries = flow->GetEntries(&entries);

	for (uint32_t index = 0; index < nbEntries; ++index)
	{
		const ComputeFlowEntry& entry = entries[index];
		for (const ComputeFlowEntryNode& entryNode : entry.Nodes)
		{
			entryNode.Node->SetInput(entryNode.InId, entry.Frame);
		}
	}

	// TODO. Add SubFlow

	const ComputeNodeImplPtr* nodes = nullptr;
	uint32_t nbNodes = flow->GetNodes(&nodes);

	// Create tasks
	std::lock_guard<std::mutex> lock(_tasksMutex);

	for (uint32_t index = 0; index < nbNodes; ++index)
	{
		ComputeNodeImplPtr node = nodes[index];
		if (node->IsValid())
		{
			// TODO. Optimize allocations
			ComputeTask* task = node->CreateComputeTask();
			assert(task != nullptr);

			ComputeTaskPtr taskPtr = ComputeTaskPtr(task);
			_tasks.push_back(taskPtr);
		}
		else
		{
			std::string message = "Invalid Node: " + std::string(node->GetName());
			Log::Error(message.c_str(), "ComputeFlow");
		}
	}

	// Bind OnCompleted
	for (ComputeTaskPtr task : _tasks)
	{
		std::weak_ptr<FlowPerformerImpl> weakPtr = GetPtr();
		task->BindOnCompleted([weakPtr](ComputeTask* task) {
			FlowPerformerImplPtr ptr = weakPtr.lock();
			if (ptr)
			{
				BasicFlowPerformerImpl* self = reinterpret_cast<BasicFlowPerformerImpl*>(ptr.get());
				self->OnTaskCompleted(task);
			}
		});
	}

	// Push tasks
	for (ComputeTaskPtr task : _tasks)
	{
		ComputeThreadPool::Instance().Push(task);
	}
}

void BasicFlowPerformerImpl::Fetch(ComputeFlowImplPtr flow)
{
	assert(flow);

	// Wait tasks
	uint32_t nbActiveTasks = GetNumActiveTasks();
	while (nbActiveTasks > 0)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		nbActiveTasks = GetNumActiveTasks();
	}

	_tasks.clear();

	// Update outputs
	const ComputeFlowOutput* outputs = nullptr;
	uint32_t nbOutputs = flow->GetOutputs(&outputs);

	for (uint32_t index = 0; index < nbOutputs; ++index)
	{
		const ComputeFlowOutput& output = outputs[index];
		for (const ComputeFlowEntryNode& outputNode : output.Nodes)
		{
			ComputeFramePtr frame = outputNode.Node->GetOutput(outputNode.InId);
			output.Frame = frame;
		}
	}
}

uint32_t BasicFlowPerformerImpl::GetNumActiveTasks() const
{
	std::lock_guard<std::mutex> lock(_tasksMutex);

	uint32_t nbActiveTasks = 0;
	for (ComputeTaskPtr task : _tasks)
	{
		if (!task->IsCompleted())
		{
			++nbActiveTasks;
		}
	}

	return nbActiveTasks;
}

void BasicFlowPerformerImpl::OnTaskCompleted(ComputeTask* task)
{
	std::lock_guard<std::mutex> lock(_tasksMutex);
	auto it = std::remove_if(_tasks.begin(), _tasks.end(), [task](ComputeTaskPtr other){
		return other.get() == task;
	});
	_tasks.erase(it, _tasks.end());

	if (_tasks.empty())
	{
		Performed();
	}
}

void BasicFlowPerformerImpl::SetOnPerformedCallback(std::function<void()> onPerformed)
{
	std::lock_guard<std::mutex> lock(_onPerformedMutex);
	_onPerformed = onPerformed;
}

void BasicFlowPerformerImpl::ClearOnPerformedCallback()
{
	std::lock_guard<std::mutex> lock(_onPerformedMutex);
	_onPerformed = nullptr;
}

void BasicFlowPerformerImpl::Performed()
{
	assert(_tasks.empty());

	std::lock_guard<std::mutex> lock(_onPerformedMutex);
	if (_onPerformed)
	{
		_onPerformed();
	}
}

} // End frox