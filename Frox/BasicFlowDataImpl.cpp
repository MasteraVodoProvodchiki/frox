#include "BasicFlowDataImpl.h"

namespace frox {

BasicFlowDataImpl::BasicFlowDataImpl()
{}
	
BasicFlowDataImpl::~BasicFlowDataImpl()
{}
	
void BasicFlowDataImpl::SetValue(const char* name, Variant value)
{
	_valuesByName[name] = value;
}

Variant BasicFlowDataImpl::GetValue(const char* name) const
{
	auto it = _valuesByName.find(name);
	return it != _valuesByName.end() ? it->second : Variant();
}

void BasicFlowDataImpl::SetValue(const Guid& guid, Variant value)
{
	_valuesByGuid[guid] = value;
}

Variant BasicFlowDataImpl::GetValue(const Guid& guid) const
{
	auto it = _valuesByGuid.find(guid);
	return it != _valuesByGuid.end() ? it->second : Variant();
}

void BasicFlowDataImpl::SetFrame(const char* name, ComputeFramePtr frame)
{
	_framesByName[name] = frame;
}

ComputeFramePtr BasicFlowDataImpl::GetFrame(const char* name) const
{
	auto it = _framesByName.find(name);
	return it != _framesByName.end() ? it->second : nullptr;
}

void BasicFlowDataImpl::SetFrame(const Guid& guid, ComputeFramePtr frame)
{
	_framesByGuid[guid] = frame;
}

ComputeFramePtr BasicFlowDataImpl::GetFrame(const Guid& guid) const
{
	auto it = _framesByGuid.find(guid);
	return it != _framesByGuid.end() ? it->second : nullptr;
}

void BasicFlowDataImpl::SetData(const char* name, ComputeDataPtr data)
{
	_datasByName[name] = data;
}

ComputeDataPtr BasicFlowDataImpl::GetData(const char* name) const
{
	auto it = _datasByName.find(name);
	return it != _datasByName.end() ? it->second : nullptr;
}

void BasicFlowDataImpl::SetData(const Guid& guid, ComputeDataPtr data)
{
	_datasByGuid[guid] = data;
}

ComputeDataPtr BasicFlowDataImpl::GetData(const Guid& guid) const
{
	auto it = _datasByGuid.find(guid);
	return it != _datasByGuid.end() ? it->second : nullptr;
}

} // End frox