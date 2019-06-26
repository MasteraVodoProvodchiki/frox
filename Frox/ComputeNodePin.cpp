#include "ComputeNodePin.h"

namespace frox {

ComputeNodePin::ComputeNodePin(ComputeNodeImpl* owner, PinType type, const std::string& name, std::function<void(ComputeFramePtr)> onChanged)
	: Owner(owner)
	, Type(type)
	, Name(name)
	, OnChanged(onChanged)
{}

ComputeNodePin::~ComputeNodePin()
{}

void ComputeNodePin::ConnectTo(ComputeNodePinPtr pin)
{
	// Add observer on Notify
	// connect SetData in pin
	std::weak_ptr<ComputeNodePin> weakPin = pin;
	std::function<void(ComputeFramePtr)> func = [weakPin](ComputeFramePtr frame) {
		ComputeNodePinPtr pin = weakPin.lock();
		if (pin)
		{
			pin->SetData(frame);
		}
	};
		
	_functions.push_back(func);
	NextPins.push_back(pin);
}

void ComputeNodePin::SetData(ComputeFramePtr frame)
{
	Frame = frame;
	Notify(frame);
}

void ComputeNodePin::Notify(ComputeFramePtr frame)
{
	for (std::function<void(ComputeFramePtr)>& func : _functions)
	{
		func(frame);
	}

	if (OnChanged)
	{
		OnChanged(frame);
	}
}

} // End frox.