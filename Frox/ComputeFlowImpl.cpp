#include "ComputeFlowImpl.h"
#include "ComputeNodeImpl.h"
#include "ComputeNodePin.h"
#include "ComputeNodeFactory.h"
#include "Log.h"
#include "ComputeThread.h"

#include <assert.h>
#include <algorithm>

namespace frox {

ComputeFlowImpl::ComputeFlowImpl()
	: _bWasInitialized(false)
{}

ComputeFlowImpl::~ComputeFlowImpl()
{
	for (ComputeNode* node : _nodes)
	{
		delete node;
	}
}

ComputeNode* ComputeFlowImpl::CreateNode(const char* type, const char* name)
{
	// Create node
	ComputeNodeInitializer initializer {
		name
	};

	ComputeNode* node = ComputeNodeFactory::Create(type, initializer);
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

void ComputeFlowImpl::DestoyNode(ComputeNode* node)
{
	// Remove From Entries/Outputs
	// TODO. Add code

	// Remove
	auto it = std::remove_if(_nodes.begin(), _nodes.end(), [node](ComputeNode* other) {
		return other == node;
	});
	_nodes.erase(it);

	delete node;
}

bool ComputeFlowImpl::ConnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId)
{
	// Impl
	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);

	// Get
	ComputeNodePinPtr outputPin = outNodeImpl->GetOutputPin(outPinId);
	ComputeNodePinPtr inputPin = inNodeImpl->GetInputPin(inPinId);

	// Connect
	outputPin->ConnectTo(inputPin);

	return true;
}

bool ComputeFlowImpl::DisconnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId)
{
	// Impl
	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);

	// Get
	ComputeNodePinPtr outputPin = outNodeImpl->GetOutputPin(outPinId);
	ComputeNodePinPtr inputPin = inNodeImpl->GetInputPin(inPinId);
	
	// Disonnect
	// TODO. Add disconnect
	outputPin; inputPin;

	assert(false);
	return false;
}

uint32_t ComputeFlowImpl::CreateEntry(const char* name)
{
	uint32_t id = uint32_t(_entries.size());
	_entries.push_back(ComputeFlowEntry{
		name != nullptr ? name : "",
	});

	return id;
}

uint32_t ComputeFlowImpl::CreateOutput(const char* name)
{
	uint32_t id = uint32_t(_outputs.size());
	_outputs.push_back(ComputeFlowOutput{
		name != nullptr ? name : "",
	});

	return id;
}

void ComputeFlowImpl::SetInput(uint32_t inId, ComputeFramePtr frame)
{
	assert(inId < _entries.size());
	_entries[inId].Frame = frame;
}

ComputeFramePtr ComputeFlowImpl::GetOutput(uint32_t outId) const
{
	assert(outId < _outputs.size());
	return _entries[outId].Frame;
}

void ComputeFlowImpl::ConnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId)
{
	assert(entryId < _entries.size());
	assert(std::find(_nodes.begin(), _nodes.end(), inNode) != _nodes.end());

	// Impl and Checl Pin
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);
	assert(inNodeImpl->GetInputPin(inPinId));

	// Append
	_entries[entryId].Nodes.push_back(ComputeFlowEntryNode{
		inNode,
		inPinId,
	});
}

void ComputeFlowImpl::DisconnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId)
{
	assert(entryId < _entries.size());

	// Remove
	auto& nodes = _entries[entryId].Nodes;
	auto it = std::remove_if(nodes.begin(), nodes.end(), [inNode, inPinId](const ComputeFlowEntryNode& nodeEntry) {
		return nodeEntry.Node == inNode && nodeEntry.InId == inPinId;
	});
	nodes.erase(it, nodes.end());
}

void ComputeFlowImpl::Initialize()
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

bool ComputeFlowImpl::WasInitialized() const
{
	return _bWasInitialized;
}

void ComputeFlowImpl::Perform()
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
	for (ComputeNode* node : _nodes)
	{
		if (node->IsValid())
		{
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

	// Push tasks
	for (ComputeTaskPtr task : _tasks)
	{
		ComputeThreadPool::Instance().Push(task);
	}
}

void ComputeFlowImpl::Fetch()
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

uint32_t ComputeFlowImpl::GetNumActiveTasks() const
{
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

} // End frox