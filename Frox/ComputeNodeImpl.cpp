#include "ComputeNodeImpl.h"
#include <assert.h>

namespace frox {

ComputeNodeImpl::ComputeNodeImpl(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{}

ComputeNodeImpl::~ComputeNodeImpl()
{}

void ComputeNodeImpl::SetInput(uint32_t inId, ComputeFramePtr frame)
{
	if (inId < _inputs.size())
	{
		_inputs[inId]->SetData(frame);
	}
}

ComputeFramePtr ComputeNodeImpl::GetOutput(uint32_t outId)
{
	return outId < _outputs.size() ? _outputs[outId]->Frame : nullptr;
}

uint32_t ComputeNodeImpl::CreateInput(const char* name)
{
	uint32_t inId = uint32_t(_inputs.size());
	_inputs.push_back(ComputeNodePin::Create(ComputeNodePin::ET_Input, name, [this, inId](ComputeFramePtr frame) {
		this->OnInputChanged(inId, frame);
	}));
	return inId;
}

uint32_t ComputeNodeImpl::CreateOutput(const char* name)
{
	uint32_t outId = uint32_t(_outputs.size());
	_outputs.push_back(ComputeNodePin::Create(ComputeNodePin::ET_Output, name));
	return outId;
}

void ComputeNodeImpl::SetOutput(uint32_t outId, ComputeFramePtr frame)
{
	assert(outId < _outputs.size());
	_outputs[outId]->SetData(frame);
}

ComputeNodePinPtr ComputeNodeImpl::GetInputPin(uint32_t inId) const
{
	assert(inId < _inputs.size());
	return _inputs[inId];
}

ComputeNodePinPtr ComputeNodeImpl::GetOutputPin(uint32_t outId) const
{
	assert(outId < _outputs.size());
	return _outputs[outId];
}

ComputeFramePtr ComputeNodeImpl::GetInput(uint32_t inId) const
{
	return GetInputPin(inId)->Frame;
}

ComputeFramePtr ComputeNodeImpl::GetOutput(uint32_t outId) const
{
	return GetOutputPin(outId)->Frame;
}

} // End frox