#include "BasicFlowDataImpl.h"

namespace frox {

BasicFlowDataImpl::BasicFlowDataImpl()
{}
	
BasicFlowDataImpl::~BasicFlowDataImpl()
{}
	
void BasicFlowDataImpl::SetFrame(const char* name, ComputeFramePtr frame)
{
	auto it = _frames.find(name);
	if (it != _frames.end())
	{
		it->second = frame;
	}
	else
	{
		_frames.insert(std::make_pair(name, frame));
	}
}

ComputeFramePtr BasicFlowDataImpl::GetFrame(const char* name) const
{
	auto it = _frames.find(name);
	return it != _frames.end() ? it->second : nullptr;
}

} // End frox