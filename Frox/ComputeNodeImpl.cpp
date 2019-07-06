#include "ComputeNodeImpl.h"

#include <algorithm>
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

int32_t ComputeNodeImpl::FindInputByName(const char* pinName) const
{
	auto it = std::find_if(_inputs.begin(), _inputs.end(), [pinName](InputPin* pin) {
		return pin->Name == pinName;
	});

	return it != _inputs.end() ? int32_t(it - _inputs.begin()) : -1;
}

int32_t ComputeNodeImpl::FindOutputByName(const char* pinName) const
{
	auto it = std::find_if(_outputs.begin(), _outputs.end(), [pinName](OutputPin* pin) {
		return pin->Name == pinName;
	});

	return it != _outputs.end() ? int32_t(it - _outputs.begin()) : -1;
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