#include "FroxImpl.h"
#include "Log.h"
#include "BasicComputeFlow.h"
#include "ComputeFrameImplV1.h"

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
}

ComputeFlow* FroxImpl::CreateComputeFlow(IComputeFlowListerner* listerner)
{
	ComputeFlow* flow =  BasicComputeFlow::Create(listerner);
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

ComputeFramePtr FroxImpl::CreateComputeFrame(Size size, ComputeFrameType type, const void* data)
{
	assert(size.Width > 0 && size.Height > 0);
	assert(type.Type != EComputeFrameType::ECFT_None && type.Channels > 0);

	return ComputeFrameImplV1::Create(size, type, data);
}

void FroxImpl::DestroyComputeFrame(ComputeFramePtr computeFrame)
{}

} // End frox