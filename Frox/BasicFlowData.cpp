#include "BasicFlowData.h"
#include "BasicFlowDataImpl.h"

#include <assert.h>

namespace frox {

BasicFlowData::BasicFlowData()
	: _impl(BasicFlowDataImpl::Create())
{}

BasicFlowData::~BasicFlowData()
{}

void BasicFlowData::SetValue(const char* name, Variant value)
{
	assert(_impl);
	return _impl->SetValue(name, value);
}

Variant BasicFlowData::GetValue(const char* name) const
{
	assert(_impl);
	return _impl->GetValue(name);
}

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

void BasicFlowData::SetData(const char* name, ComputeDataPtr data)
{
	assert(_impl);
	return _impl->SetData(name, data);
}

ComputeDataPtr BasicFlowData::GetData(const char* name) const
{
	assert(_impl);
	return _impl->GetData(name);
}

} // End frox