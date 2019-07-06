#include "BasicComputeFlowImpl.h"
#include "ComputeNodeImpl.h"
#include "ComputeNodePin.h"
#include "ComputeNodeFactory.h"
#include "Log.h"
#include "ComputeThread.h"

#include <assert.h>
#include <algorithm>

namespace frox {

namespace utils {

/*
void SortNodes(const std::vector<ComputeNodeImplPtr>& in, std::vector<ComputeNodeImplPtr>& out)
{
	out.reserve(in.size());
	
	for (ComputeNodeImplPtr node : in)
	{
		size_t nearestIndex = std::numeric_limits<size_t>::max();
		for (ComputeNodePinPtr pin : node->GetOutputPins())
		{
			for (ComputeNodePinPtr nextPin : pin->NextPins)
			{
				ComputeNodeImplPtr nextNode = nextPin->Owner->getptr();
				auto it = std::find(out.begin(), out.end(), nextNode);
				if (it != out.end() && nearestIndex > (it - out.begin()))
				{
					nearestIndex = it - out.begin();
				}
			}
		}
		
		if (nearestIndex != std::numeric_limits<size_t>::max())
		{
			out.insert(out.begin() + nearestIndex, node);
		}
		else
		{
			out.push_back(node);
		}
	}
}
*/

std::vector<Pin*> FilterNextPins(OutputPin* pin, const std::vector<BasicComputeFlowImpl::Connection>& connections)
{
	std::vector<Pin*> out;

	for (auto& connection : connections)
	{
		if (connection.OutputPin == pin)
		{
			out.push_back(connection.InputPin);
		}	
	}

	return out;
}

void SortNodes(const std::vector<ComputeNodeImplPtr>& in, const std::vector<BasicComputeFlowImpl::Connection>& connections, std::vector<ComputeNodeImplPtr>& out)
{
	out.reserve(in.size());

	for (ComputeNodeImplPtr node : in)
	{
		size_t nearestIndex = std::numeric_limits<size_t>::max();
		for (OutputPin* pin : node->GetOutputPins())
		{
			std::vector<Pin*> nextPins = FilterNextPins(pin, connections);
			for (Pin* nextPin : nextPins)
			{
				ComputeNodeImplPtr nextNode = nextPin->Owner->getptr();
				auto it = std::find(out.begin(), out.end(), nextNode);
				if (it != out.end() && nearestIndex > (it - out.begin()))
				{
					nearestIndex = it - out.begin();
				}
			}
		}

		if (nearestIndex != std::numeric_limits<size_t>::max())
		{
			out.insert(out.begin() + nearestIndex, node);
		}
		else
		{
			out.push_back(node);
		}
	}
}

} // End utils

BasicComputeFlowImpl::BasicComputeFlowImpl()
	: _bDirty(true)
{}

BasicComputeFlowImpl::~BasicComputeFlowImpl()
{}

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
	ComputeNodeImplPtr nodePtr = std::shared_ptr<ComputeNodeImpl>(node);
	_nodes.push_back(nodePtr);

	MakeDirty();

	return node;
}

void BasicComputeFlowImpl::DestoyNode(ComputeNodeImpl* node)
{
	// Remove From Entries/Outputs
	// TODO. Add code
	DisconnectAll(node);

	// Remove
	auto it = std::remove_if(_nodes.begin(), _nodes.end(), [node](ComputeNodeImplPtr other) {
		return other.get() == node;
	});
	_nodes.erase(it);

	MakeDirty();
}

bool BasicComputeFlowImpl::ConnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	// Get
	Pin* outputPin = outNode->GetOutputPin(outPinId);
	Pin* inputPin = inNode->GetInputPin(inPinId);
	assert(outputPin != nullptr && inputPin != nullptr);

	// Connect
	inputPin->ConnectFrom(outputPin);

	_connections.push_back(Connection{
		outNode,
		outputPin,
		inNode,
		inputPin
	});

	MakeDirty();

	return true;
}

bool BasicComputeFlowImpl::DisconnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	// Get
	Pin* outputPin = outNode->GetOutputPin(outPinId);
	Pin* inputPin = inNode->GetInputPin(inPinId);
	assert(outputPin != nullptr && inputPin != nullptr);

	// Disonnect
	auto it = std::remove_if(_connections.begin(), _connections.end(), [outNode, outputPin, inNode, inputPin](const Connection& connection) {
		return
			connection.OutNode == outNode &&
			connection.OutputPin == outputPin &&
			connection.InNode == inNode &&
			connection.InputPin == inputPin;
	});
	_connections.erase(it, _connections.end());
	
	// TODO. Add disconnect
	outputPin; inputPin;

	MakeDirty();

	assert(false);
	return false;
}

uint32_t BasicComputeFlowImpl::CreateEntry(const char* name)
{
	uint32_t id = uint32_t(_entries.size());
	_entries.push_back(ComputeFlowEntry{
		name != nullptr ? name : "",
	});
	MakeDirty();

	return id;
}

uint32_t BasicComputeFlowImpl::CreateOutput(const char* name)
{
	uint32_t id = uint32_t(_outputs.size());
	_outputs.push_back(ComputeFlowOutput{
		name != nullptr ? name : "",
	});
	MakeDirty();

	return id;
}

uint32_t BasicComputeFlowImpl::GetEntries(const ComputeFlowEntry** outEntries) const
{
	*outEntries = _entries.data();
	return uint32_t(_entries.size());
}

uint32_t BasicComputeFlowImpl::GetOutputs(const ComputeFlowOutput** outOutputs) const
{
	*outOutputs = _outputs.data();
	return uint32_t(_outputs.size());
}

int32_t BasicComputeFlowImpl::FindEntryByName(const char* name) const
{
	auto it = std::find_if(_entries.begin(), _entries.end(), [name](const ComputeFlowEntry& entry) {
		return entry.Name == name;
	});
	return it != _entries.end() ? it - _entries.begin() : -1;
}

int32_t BasicComputeFlowImpl::FindOutputByName(const char* name) const
{
	auto it = std::find_if(_outputs.begin(), _outputs.end(), [name](const ComputeFlowOutput& output) {
		return output.Name == name;
	});
	return it != _outputs.end() ? it - _outputs.begin() : -1;
}

void BasicComputeFlowImpl::ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	assert(entryId < _entries.size());
	assert(std::find(_nodes.begin(), _nodes.end(), inNode->getptr()) != _nodes.end());

	// Check Pin
	Pin* pin = inNode->GetInputPin(inPinId);
	assert(pin);

	// Append
	_entries[entryId].Nodes.push_back(ComputeFlowEntryNode{
		inNode,
		pin->Id,
	});

	pin->ConnectFrom(pin->Id);

	MakeDirty();
}

void BasicComputeFlowImpl::DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId)
{
	assert(entryId < _entries.size());

	// Check Pin
	Pin* pin = inNode->GetInputPin(inPinId);
	assert(pin);

	// Remove
	auto& nodes = _entries[entryId].Nodes;
	auto it = std::remove_if(nodes.begin(), nodes.end(), [inNode, pin](const ComputeFlowEntryNode& nodeEntry) {
		return nodeEntry.Node == inNode && nodeEntry.PinId == pin->Id;
	});
	nodes.erase(it, nodes.end());

	MakeDirty();
}

void BasicComputeFlowImpl::ConnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId)
{
	assert(outputId < _outputs.size());

	assert(std::find(_nodes.begin(), _nodes.end(), outNode->getptr()) != _nodes.end());

	// Check Pin
	Pin* pin = outNode->GetOutputPin(outPinId);
	assert(pin);

	// Append
	_outputs[outputId].Nodes.push_back(ComputeFlowEntryNode{
		outNode,
		pin->Id,
	});

	MakeDirty();
}

void BasicComputeFlowImpl::DisconnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId)
{
	assert(outputId < _outputs.size());

	// Check Pin
	Pin* pin = outNode->GetOutputPin(outPinId);
	assert(pin);

	// Remove
	auto& nodes = _outputs[outputId].Nodes;
	auto it = std::remove_if(nodes.begin(), nodes.end(), [outNode, pin](const ComputeFlowEntryNode& nodeEntry) {
		return nodeEntry.Node == outNode && nodeEntry.PinId == pin->Id;
	});
	nodes.erase(it, nodes.end());

	MakeDirty();
}

void BasicComputeFlowImpl::Prepare()
{
	if (_bDirty)
	{
		_sortedNodes.clear();
		utils::SortNodes(_nodes, _connections, _sortedNodes);

		_bDirty = false;
	}
}

uint32_t BasicComputeFlowImpl::GetNodes(const ComputeNodeImplPtr** outNodes) const
{
	*outNodes = _sortedNodes.data();
	return uint32_t(_sortedNodes.size());
}

void BasicComputeFlowImpl::DisconnectAll(ComputeNodeImpl* node)
{
	auto it = std::remove_if(_connections.begin(), _connections.end(), [node](const Connection& connection) {
		return connection.OutNode == node || connection.InNode == node;
	});
	_connections.erase(it, _connections.end());
}

void BasicComputeFlowImpl::MakeDirty()
{
	_bDirty = true;
}

} // End frox