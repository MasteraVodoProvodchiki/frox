#include "BasicFlowPerformer.h"
#include "BasicFlowPerformerImpl.h"
#include "BasicComputeFlow.h"

#include <assert.h>

namespace frox {

BasicFlowPerformer::BasicFlowPerformer(IComputeFlowListerner* listerner)
	: _impl(BasicFlowPerformerImpl::Create())
	, _listerner(listerner)
{
	BasicFlowPerformerImpl* impl = reinterpret_cast<BasicFlowPerformerImpl*>(_impl.get());
	impl->SetOnPerformedCallback(std::bind(&BasicFlowPerformer::OnPerformed, this));
}

BasicFlowPerformer::~BasicFlowPerformer()
{
	BasicFlowPerformerImpl* impl = reinterpret_cast<BasicFlowPerformerImpl*>(_impl.get());
	impl->ClearOnPerformedCallback();
}

void BasicFlowPerformer::Perform(ComputeFlow* flow)
{
	assert(flow != nullptr);

	BasicComputeFlow* basicFlow = reinterpret_cast<BasicComputeFlow*>(flow);

	assert(_impl);
	_impl->Perform(basicFlow->GetComputeFlow());
}

void BasicFlowPerformer::Fetch(ComputeFlow* flow)
{
	assert(flow != nullptr);

	BasicComputeFlow* basicFlow = reinterpret_cast<BasicComputeFlow*>(flow);

	assert(_impl);
	_impl->Fetch(basicFlow->GetComputeFlow());
}

uint32_t BasicFlowPerformer::GetNumActiveTasks() const
{
	assert(_impl);
	return _impl->GetNumActiveTasks();

}

void BasicFlowPerformer::OnPerformed()
{
	if (_listerner != nullptr)
	{
		_listerner->OnPerformed();
	}
}

} // End frox