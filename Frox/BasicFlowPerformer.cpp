#include "BasicFlowPerformer.h"
#include "BasicFlowPerformerImpl.h"
#include "BasicComputeFlow.h"
#include "BasicFlowData.h"
#include "BasicFlowDataImpl.h"
#include "DebugFlowDataImpl.h"

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

void BasicFlowPerformer::Perform(ComputeFlow* flow, FlowData* inputData)
{
	assert(flow != nullptr);

	BasicComputeFlow* basicFlow = reinterpret_cast<BasicComputeFlow*>(flow);
	BasicFlowData* basicInputData = reinterpret_cast<BasicFlowData*>(inputData);

	assert(_impl);
	_impl->Perform(basicFlow->GetComputeFlow(), basicInputData->GetData());
}

void BasicFlowPerformer::Fetch(ComputeFlow* flow, FlowData* outputData)
{
	assert(flow != nullptr);

	BasicComputeFlow* basicFlow = reinterpret_cast<BasicComputeFlow*>(flow);
	BasicFlowData* basicOutputData = reinterpret_cast<BasicFlowData*>(outputData);

	assert(_impl);
	_impl->Fetch(basicFlow->GetComputeFlow(), basicOutputData->GetData());
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

DebugFlowDataPtr BasicFlowPerformer::GetLatestPerformedDataForDebug() const
{
#ifdef FROX_DEBUG
	assert(_impl);
	return std::make_unique<DebugFlowDataImpl>(_impl->GetImmidiatePerformerData());
#else
	assert(false);
	return std::make_unique<DebugFlowDataImpl>(nullptr);
#endif
}

} // End frox