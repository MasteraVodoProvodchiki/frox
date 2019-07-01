#include "BasicFlowData.h"
#include "BasicFlowDataImpl.h"

#include <assert.h>

namespace frox {

BasicFlowData::BasicFlowData()
	: _impl(BasicFlowDataImpl::Create())
{}

BasicFlowData::~BasicFlowData()
{}

void BasicFlowData::SetFrame(const char* name, ComputeFrame* frame)
{
	assert(_impl);
	return _impl->SetFrame(name, frame);
}

} // End frox