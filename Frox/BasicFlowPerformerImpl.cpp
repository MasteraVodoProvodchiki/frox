#include "BasicFlowPerformerImpl.h"
#include "BasicFlowDataImpl.h"

#include "Log.h"
#include "ComputeThread.h"
#include "ComputeNodeImpl.h"

#include <assert.h>
#include <algorithm>

namespace frox {

BasicFlowPerformerImpl::BasicFlowPerformerImpl()
	: _privateData(BasicFlowDataImpl::Create())
{}

BasicFlowPerformerImpl::~BasicFlowPerformerImpl()
{}

void BasicFlowPerformerImpl::Perform(ComputeFlowImplPtr flow, FlowDataImplPtr inputData)
{
	assert(flow);
	assert(_privateData);

	flow->Prepare();

	// Update entries
	if (inputData)
	{
		const ComputeFlowEntry* entries = nullptr;
		uint32_t nbEntries = flow->GetEntries(&entries);

		for (uint32_t index = 0; index < nbEntries; ++index)
		{
			const ComputeFlowEntry& entry = entries[index];
			switch (entry.Type)
			{
			case EPinValueType::Frame: {
				ComputeFramePtr frame = inputData->GetFrame(entry.Name.data());
				for (const ComputeFlowEntryNode& entryNode : entry.Nodes)
				{
					_privateData->SetFrame(entryNode.PinId, frame);
				}
				break;
			}	
			case EPinValueType::Value: {
				Variant value = inputData->GetValue(entry.Name.data());
				for (const ComputeFlowEntryNode& entryNode : entry.Nodes)
				{
					_privateData->SetValue(entryNode.PinId, value);
				}
			}	
			case EPinValueType::Data: {
				ComputeDataPtr data = inputData->GetData(entry.Name.data());
				for (const ComputeFlowEntryNode& entryNode : entry.Nodes)
				{
					_privateData->SetData(entryNode.PinId, data);
				}
			}
			}	
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
			ComputeTask* task = node->CreateComputeTask(_privateData, _privateData);
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

void BasicFlowPerformerImpl::Fetch(ComputeFlowImplPtr flow, FlowDataImplPtr outputData)
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
	if (outputData)
	{
		const ComputeFlowOutput* outputs = nullptr;
		uint32_t nbOutputs = flow->GetOutputs(&outputs);

		for (uint32_t index = 0; index < nbOutputs; ++index)
		{
			const ComputeFlowOutput& output = outputs[index];
			switch (output.Type)
			{
			case EPinValueType::Frame: {
				for (const ComputeFlowEntryNode& outputNode : output.Nodes)
				{
					ComputeFramePtr frame = _privateData->GetFrame(outputNode.PinId);
					outputData->SetFrame(output.Name.c_str(), frame);
				}
				break;
			}
			case EPinValueType::Value: {
				for (const ComputeFlowEntryNode& outputNode : output.Nodes)
				{
					Variant value = _privateData->GetValue(outputNode.PinId);
					outputData->SetValue(output.Name.c_str(), value);
				}
			}
			case EPinValueType::Data: {
				for (const ComputeFlowEntryNode& outputNode : output.Nodes)
				{
					ComputeDataPtr data = _privateData->GetData(outputNode.PinId);
					outputData->SetData(output.Name.c_str(), data);
				}
			}
			}
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