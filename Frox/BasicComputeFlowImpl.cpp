#include "BasicComputeFlowImpl.h"
#include "ComputeNodeImpl.h"
#include "ComputeNodePin.h"
#include "ComputeNodeFactory.h"
#include "Log.h"
#include "ComputeThread.h"

#include <assert.h>
#include <algorithm>

namespace frox {

BasicComputeFlowImpl::BasicComputeFlowImpl()
	: _bWasInitialized(false)
{}

BasicComputeFlowImpl::~BasicComputeFlowImpl()
{
	for (ComputeNodeImpl* node : _nodes)
	{
		delete node;
	}
}

ComputeNodeImpl* BasicComputeFlowImpl::CreateNode(const char* type, const char* name)
{
	// Create node
	ComputeNodeInitializer initializer {
		name
	};

	ComputeNode* nodeTemp = ComputeNodeFactory::Create(type, initializer);

	ComputeNodeImpl* node = reinterpret_cast<ComputeNodeImpl*>(nodeTemp);
	if (node == nullptr)
	{
		Log::Error("Unknown ComputeNode type", "ComputeFlow");
		return nullptr;
	}

	// Pins
	node->AllocateDefaultPins();

	// Append
	_nodes.push_back(node);

	return node;
}

void BasicComputeFlowImpl::DestoyNode(ComputeNodeImpl* node)
{
	// Remove From Entries/Outputs
	// TODO. Add code

	// Remove
	auto it = std::remove_if(_nodes.begin(), _nodes.end(), [node](ComputeNodeImpl* other) {
		return other == node;
	});
	_nodes.erase(it);

	delete node;
}

bool BasicComputeFlowImpl::ConnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	// Get
	ComputeNodePinPtr outputPin = outNode->GetOutputPin(outPinId);
	ComputeNodePinPtr inputPin = inNode->GetInputPin(inPinId);

	// Connect
	outputPin->ConnectTo(inputPin);

	return true;
}

bool BasicComputeFlowImpl::DisconnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	// Get
	ComputeNodePinPtr outputPin = outNode->GetOutputPin(outPinId);
	ComputeNodePinPtr inputPin = inNode->GetInputPin(inPinId);
	
	// Disonnect
	// TODO. Add disconnect
	outputPin; inputPin;

	assert(false);
	return false;
}

uint32_t BasicComputeFlowImpl::CreateEntry(const char* name)
{
	uint32_t id = uint32_t(_entries.size());
	_entries.push_back(ComputeFlowEntry{
		name != nullptr ? name : "",
	});

	return id;
}

uint32_t BasicComputeFlowImpl::CreateOutput(const char* name)
{
	uint32_t id = uint32_t(_outputs.size());
	_outputs.push_back(ComputeFlowOutput{
		name != nullptr ? name : "",
	});

	return id;
}

void BasicComputeFlowImpl::SetInput(uint32_t inId, ComputeFramePtr frame)
{
	assert(inId < _entries.size());
	_entries[inId].Frame = frame;
}

ComputeFramePtr BasicComputeFlowImpl::GetOutput(uint32_t outId) const
{
	assert(outId < _outputs.size());
	return _entries[outId].Frame;
}

void BasicComputeFlowImpl::ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	assert(entryId < _entries.size());
	assert(std::find(_nodes.begin(), _nodes.end(), inNode) != _nodes.end());

	// Check Pin
	assert(inNode->GetInputPin(inPinId));

	// Append
	_entries[entryId].Nodes.push_back(ComputeFlowEntryNode{
		inNode,
		inPinId,
	});
}

void BasicComputeFlowImpl::DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	assert(entryId < _entries.size());

	// Remove
	auto& nodes = _entries[entryId].Nodes;
	auto it = std::remove_if(nodes.begin(), nodes.end(), [inNode, inPinId](const ComputeFlowEntryNode& nodeEntry) {
		return nodeEntry.Node == inNode && nodeEntry.InId == inPinId;
	});
	nodes.erase(it, nodes.end());
}

void BasicComputeFlowImpl::Initialize()
{
	for (ComputeNode* node : _nodes)
	{
		if (!node->WasInitialized())
		{
			node->Initialize();
		}
	}

	_bWasInitialized = true;
}

bool BasicComputeFlowImpl::WasInitialized() const
{
	return _bWasInitialized;
}

void BasicComputeFlowImpl::Perform()
{
	if (!WasInitialized())
	{
		Initialize();
	}

	// Update entries
	for (const ComputeFlowEntry& entry : _entries)
	{
		for (const ComputeFlowEntryNode& entryNode : entry.Nodes)
		{
			entryNode.Node->SetInput(entryNode.InId, entry.Frame);
		}
	}

	// TODO. Add SubFlow
	// Create tasks
	std::lock_guard<std::mutex> lock(_tasksMutex);
	for (ComputeNode* node : _nodes)
	{
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
		std::weak_ptr<ComputeFlowImpl> weakPtr = GetPtr();
		task->BindOnCompleted([weakPtr](ComputeTask* task) {
			ComputeFlowImplPtr ptr = weakPtr.lock();
			if (ptr)
			{
				BasicComputeFlowImpl* self = reinterpret_cast<BasicComputeFlowImpl*>(ptr.get());
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

void BasicComputeFlowImpl::Fetch()
{
	// Wait tasks
	uint32_t nbActiveTasks = GetNumActiveTasks();
	while (nbActiveTasks > 0)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		nbActiveTasks = GetNumActiveTasks();
	}

	_tasks.clear();
}

uint32_t BasicComputeFlowImpl::GetNumActiveTasks() const
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

void BasicComputeFlowImpl::OnTaskCompleted(ComputeTask* task)
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

void BasicComputeFlowImpl::SetOnPerformedCallback(std::function<void()> onPerformed)
{
	std::lock_guard<std::mutex> lock(_onPerformedMutex);
	_onPerformed = onPerformed;
}

void BasicComputeFlowImpl::ClearOnPerformedCallback()
{
	std::lock_guard<std::mutex> lock(_onPerformedMutex);
	_onPerformed = nullptr;
}

void BasicComputeFlowImpl::Performed()
{
	assert(_tasks.empty());

	std::lock_guard<std::mutex> lock(_onPerformedMutex);
	if (_onPerformed)
	{
		_onPerformed();
	}
}

} // End frox