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