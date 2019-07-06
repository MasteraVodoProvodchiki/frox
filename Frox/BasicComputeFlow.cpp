#include "BasicComputeFlow.h"
#include "ComputeNodeImpl.h"

#include <assert.h>

namespace frox {

BasicComputeFlow::BasicComputeFlow()
	: _impl(BasicComputeFlowImpl::Create())
{}

BasicComputeFlow::~BasicComputeFlow()
{}

ComputeNode* BasicComputeFlow::CreateNode(const char* type, const char* name)
{
	assert(_impl);
	return _impl->CreateNode(type, name);
}

void BasicComputeFlow::DestoyNode(ComputeNode* node)
{
	assert(_impl);

	ComputeNodeImpl* nodeImpl = reinterpret_cast<ComputeNodeImpl*>(node);
	_impl->DestoyNode(nodeImpl);
}

bool BasicComputeFlow::ConnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId)
{
	assert(_impl);

	// Impl
	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);

	return _impl->ConnectNodes(outNodeImpl, outPinId, inNodeImpl, inPinId);
}

bool BasicComputeFlow::DisconnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId)
{
	assert(_impl);

	// Impl
	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);

	return _impl->DisconnectNodes(outNodeImpl, outPinId, inNodeImpl, inPinId);
}

uint32_t BasicComputeFlow::CreateEntry(const char* name)
{
	assert(_impl);
	return _impl->CreateEntry(name);
}

uint32_t BasicComputeFlow::CreateOutput(const char* name)
{
	assert(_impl);
	return _impl->CreateOutput(name);
}

int32_t BasicComputeFlow::FindEntryByName(const char* name) const
{
	assert(_impl);
	return _impl->FindEntryByName(name);
}

int32_t BasicComputeFlow::FindOutputByName(const char* name) const
{
	assert(_impl);
	return _impl->FindOutputByName(name);
}

void BasicComputeFlow::ConnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId)
{
	assert(_impl);

	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);
	return _impl->ConnectEntry(entryId, inNodeImpl, inPinId);
}

void BasicComputeFlow::DisconnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId)
{
	assert(_impl);

	ComputeNodeImpl* inNodeImpl = reinterpret_cast<ComputeNodeImpl*>(inNode);
	return _impl->DisconnectEntry(entryId, inNodeImpl, inPinId);
}

void BasicComputeFlow::ConnectOutput(uint32_t outputId, ComputeNode* outNode, uint32_t outPinId)
{
	assert(_impl);

	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	return _impl->ConnectOutput(outputId, outNodeImpl, outPinId);
}

void BasicComputeFlow::DisconnectOutput(uint32_t outputId, ComputeNode* outNode, uint32_t outPinId)
{
	assert(_impl);

	ComputeNodeImpl* outNodeImpl = reinterpret_cast<ComputeNodeImpl*>(outNode);
	return _impl->DisconnectEntry(outputId, outNodeImpl, outPinId);
}

} // End frox