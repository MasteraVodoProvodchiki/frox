#include "ComputeNodeImpl.h"

#include <assert.h>

namespace frox {

ComputeNodeImpl::ComputeNodeImpl(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _name(initializer.Name != nullptr ? initializer.Name : "Noname")
{}

ComputeNodeImpl::~ComputeNodeImpl()
{}

const char* ComputeNodeImpl::GetName() const
{
	return _name.c_str();
}

void ComputeNodeImpl::AllocateDefaultPins()
{}

bool ComputeNodeImpl::IsValid() const
{
	return true;
}

ComputeTask* ComputeNodeImpl::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	return nullptr;
}

void ComputeNodeImpl::ReallocatePins()
{
	_inputs.clear();
	_outputs.clear();

	this->AllocateDefaultPins();
}

void ComputeNodeImpl::RegisterInput(InputPin* pin)
{
	pin->Owner = this;
	_inputs.push_back(pin);
}

void ComputeNodeImpl::RegisterOutput(OutputPin* pin)
{
	pin->Owner = this;
	_outputs.push_back(pin);
}

InputPin* ComputeNodeImpl::GetInputPin(uint32_t inId) const
{
	assert(inId < _inputs.size());
	return _inputs[inId];
}

OutputPin* ComputeNodeImpl::GetOutputPin(uint32_t outId) const
{
	assert(outId < _outputs.size());
	return _outputs[outId];
}

} // End frox