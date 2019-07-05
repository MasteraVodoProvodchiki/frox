#include "BasicFlowDataImpl.h"

namespace frox {

BasicFlowDataImpl::BasicFlowDataImpl()
{}
	
BasicFlowDataImpl::~BasicFlowDataImpl()
{}
	
void BasicFlowDataImpl::SetFrame(const char* name, ComputeFramePtr frame)
{
	auto it = _framesByName.find(name);
	if (it != _framesByName.end())
	{
		it->second = frame;
	}
	else
	{
		_framesByName.insert(std::make_pair(name, frame));
	}
}

ComputeFramePtr BasicFlowDataImpl::GetFrame(const char* name) const
{
	auto it = _framesByName.find(name);
	return it != _framesByName.end() ? it->second : nullptr;
}

void BasicFlowDataImpl::SetFrame(const Guid& guid, ComputeFramePtr frame)
{
	auto it = _framesByGuid.find(guid);
	if (it != _framesByGuid.end())
	{
		it->second = frame;
	}
	else
	{
		_framesByGuid.insert(std::make_pair(guid, frame));
	}
}

ComputeFramePtr BasicFlowDataImpl::GetFrame(const Guid& guid) const
{
	auto it = _framesByGuid.find(guid);
	return it != _framesByGuid.end() ? it->second : nullptr;
}

} // End frox