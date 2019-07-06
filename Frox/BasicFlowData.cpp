#include "BasicFlowData.h"
#include "BasicFlowDataImpl.h"

#include <assert.h>

namespace frox {

BasicFlowData::BasicFlowData()
	: _impl(BasicFlowDataImpl::Create())
{}

BasicFlowData::~BasicFlowData()
{}

void BasicFlowData::SetFrame(const char* name, ComputeFramePtr frame)
{
	assert(_impl);
	return _impl->SetFrame(name, frame);
}

ComputeFramePtr BasicFlowData::GetFrame(const char* name) const
{
	assert(_impl);
	return _impl->GetFrame(name);
}

} // End frox