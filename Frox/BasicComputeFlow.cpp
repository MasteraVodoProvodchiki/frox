#include "BasicComputeFlow.h"
#include "ComputeNodeImpl.h"

#include <assert.h>

namespace frox {

BasicComputeFlow::BasicComputeFlow(IComputeFlowListerner* listerner)
	: _impl(BasicComputeFlowImpl::Create())
	, _listerner(listerner)
{
	BasicComputeFlowImpl* impl = reinterpret_cast<BasicComputeFlowImpl*>(_impl.get());
	impl->SetOnPerformedCallback(std::bind(&BasicComputeFlow::OnPerformed, this));
}

BasicComputeFlow::~BasicComputeFlow()
{
	BasicComputeFlowImpl* impl = reinterpret_cast<BasicComputeFlowImpl*>(_impl.get());
	impl->ClearOnPerformedCallback();
}

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

void BasicComputeFlow::SetInput(uint32_t inId, ComputeFramePtr frame)
{
	assert(_impl);
	return _impl->SetInput(inId, frame);
}

ComputeFramePtr BasicComputeFlow::GetOutput(uint32_t outId) const
{
	assert(_impl);
	return _impl->GetOutput(outId);
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

void BasicComputeFlow::Initialize()
{
	assert(_impl);
	_impl->Initialize();
}

bool BasicComputeFlow::WasInitialized() const
{
	assert(_impl);
	return _impl->WasInitialized();
}

void BasicComputeFlow::Perform()
{
	assert(_impl);
	_impl->Perform();
}

void BasicComputeFlow::Fetch()
{
	assert(_impl);
	_impl->Fetch();
}

uint32_t BasicComputeFlow::GetNumActiveTasks() const
{
	assert(_impl);
	return _impl->GetNumActiveTasks();

}

void BasicComputeFlow::OnPerformed()
{
	if (_listerner != nullptr)
	{
		_listerner->OnPerformed();
	}
}

} // End frox