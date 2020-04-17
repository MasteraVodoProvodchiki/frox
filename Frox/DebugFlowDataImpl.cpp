#include "DebugFlowDataImpl.h"

namespace frox {

#ifdef FROX_DEBUG
void DebugFlowDataImpl::SetValue(const Guid& guid, Variant value)
{
	_source->SetValue(guid, value);
}

Variant DebugFlowDataImpl::GetValue(const Guid& guid) const
{
	return _source->GetValue(guid);
}

void DebugFlowDataImpl::SetFrame(const Guid& guid, ComputeFramePtr frame)
{
	_source->SetFrame(guid, frame);
}

ComputeFramePtr DebugFlowDataImpl::GetFrame(const Guid& guid) const
{
	return _source->GetFrame(guid);
}

void DebugFlowDataImpl::SetData(const Guid& guid, ComputeDataPtr data)
{
	_source->SetData(guid, data);
}

ComputeDataPtr DebugFlowDataImpl::GetData(const Guid& guid) const
{
	return _source->GetData(guid);
}
#else
void DebugFlowDataImpl::SetValue(const Guid& guid, Variant value)
{
	assert(false);
}

Variant DebugFlowDataImpl::GetValue(const Guid& guid) const
{
	assert(false);
	return Variant();
}

void DebugFlowDataImpl::SetFrame(const Guid& guid, ComputeFramePtr frame)
{
	assert(false);
}

ComputeFramePtr DebugFlowDataImpl::GetFrame(const Guid& guid) const
{
	assert(false);
	return nullptr;
}

void DebugFlowDataImpl::SetData(const Guid& guid, ComputeDataPtr data)
{
	assert(false);
}

ComputeDataPtr DebugFlowDataImpl::GetData(const Guid& guid) const
{
	assert(false);
	return nullptr;
}
#endif // FROX_DEBUG

} // End frox