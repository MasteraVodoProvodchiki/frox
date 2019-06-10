#include "ComputeFlowImpl.h"
#include "ComputeNodeImpl.h"
#include "ComputeNodePin.h"
#include "ComputeTask.h"
#include "ComputeNodeFactory.h"
#include "Log.h"
#include "ComputeThread.h"

#include <assert.h>
#include <algorithm>

namespace frox {

ComputeFlowImpl::ComputeFlowImpl()
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
	ComputeNodeInitializer initializer {
		name
	};

	ComputeNode* node = ComputeNodeFactory::Create(type, initializer);
	if (node == nullptr)
	{
		Log::Error("Unknown ComputeNode type", "ComputeFlow");
		return nullptr;
	}

	node->AllocateDefaultPins();

	_nodes.push_back(node);
	return node;
}

void ComputeFlowImpl::DestoyNode(ComputeNode* node)
{
	auto it = std::remove_if(_nodes.begin(), _nodes.end(), [node](ComputeNode* other) {
		return other == node;
	});
	_nodes.erase(it);

	delete node;
}

bool ComputeFlowImpl::ConnectNodes(ComputeNode* outNode, int outPinId, ComputeNode* inNode, int inPinId)
{
	// Impl
	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);

	// Get
	ComputeNodePinPtr outputPin = outNodeImpl->GetOutputPin(outPinId);
	ComputeNodePinPtr inputPin = inNodeImpl->GetInputPin(inPinId);

	// connect
	outputPin->ConnectTo(inputPin);


	return true;
}

bool ComputeFlowImpl::DisconnectNodes(ComputeNode* outNode, int outPinId, ComputeNode* inNode, int inPinId)
{
	// Impl
	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);

	// Get
	ComputeNodePinPtr outputPin = outNodeImpl->GetOutputPin(outPinId);
	ComputeNodePinPtr inputPin = inNodeImpl->GetInputPin(inPinId);
	
	// disonnect
	outputPin; inputPin;

	return true;
}

void ComputeFlowImpl::Perform()
{
	for (ComputeNode* node : _nodes)
	{
		ComputeTask* task = node->CreateComputeTask();
		ComputeThreadPool::Instance().Push(task);
	}

	// Connect to tasks
}

void ComputeFlowImpl::Fetch()
{
	// Wait tasks
}

} // End frox