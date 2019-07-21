#include "FroxImpl.h"
#include "Log.h"
#include "BasicComputeFlow.h"
#include "BasicFlowPerformer.h"
#include "BasicFlowData.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#else
#include "ComputeFrameImplV1.h"
#endif

#include <algorithm>
#include <assert.h>

namespace frox {

FroxImpl::FroxImpl(const FroxDesc& desc)
{
	Log::SetListerner(desc.LogListerner);
}

FroxImpl::~FroxImpl()
{
	assert(_flows.empty());
	assert(_datas.empty());
	assert(_performers.empty());
}

ComputeFlow* FroxImpl::CreateComputeFlow()
{
	ComputeFlow* flow =  BasicComputeFlow::Create();
	_flows.push_back(flow);
	return flow;
}

void FroxImpl::DestroyComputeFlow(ComputeFlow* computeFlow)
{
	auto it = std::remove_if(_flows.begin(), _flows.end(), [computeFlow](ComputeFlow* other) {
		return other == computeFlow;
	});
	_flows.erase(it);

	delete computeFlow;
}

FlowData* FroxImpl::CreateFlowData()
{
	FlowData* data = BasicFlowData::Create();
	_datas.push_back(data);
	return data;
}

void FroxImpl::DestroyFlowData(FlowData* flowData)
{
	auto it = std::remove_if(_datas.begin(), _datas.end(), [flowData](FlowData* other) {
		return other == flowData;
	});
	_datas.erase(it);

	delete flowData;
}

FlowPerformer* FroxImpl::CreateFlowPerformer(IComputeFlowListerner* listerner)
{
	FlowPerformer* performer = BasicFlowPerformer::Create(listerner);
	_performers.push_back(performer);
	return performer;
}

void FroxImpl::DestroyFlowPerformer(FlowPerformer* performer)
{
	auto it = std::remove_if(_performers.begin(), _performers.end(), [performer](FlowPerformer* other) {
		return other == performer;
	});
	_performers.erase(it);

	delete performer;
}

ComputeFramePtr FroxImpl::CreateComputeFrame(Size size, ComputeFrameType type, const void* data)
{
	assert(size.Width > 0 && size.Height > 0);
	assert(type.Type != EComputeFrameType::ECFT_None && type.Channels > 0);

#ifndef WITHOUT_OPENCV
	return OpenCVComputeFrameImpl::Create(size, type, data);
#else
	return ComputeFrameImplV1::Create(size, type, data);
#endif
}

void FroxImpl::DestroyComputeFrame(ComputeFramePtr computeFrame)
{}

} // End frox