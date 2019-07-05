#pragma once

#include "ComputeFrame.h"
#include "Guid.h"

#include <vector>
#include <string>
#include <functional>
#include <memory>

namespace frox {

class ComputeNodeImpl;

using ComputeNodePinPtr = std::shared_ptr<class ComputeNodePin>;
class ComputeNodePin
{
public:
	enum PinType
	{
		ET_Input,
		ET_Output
	};

public:
	ComputeNodePin(ComputeNodeImpl* owner, PinType type, const std::string& name/*, std::function<void(ComputeFramePtr)> onChanged*/);
	~ComputeNodePin();
	
	static ComputeNodePinPtr Create(ComputeNodeImpl* owner, PinType type, const std::string& name/*, std::function<void(ComputeFramePtr)> onChanged = std::function<void(ComputeFramePtr)>()*/)
	{
		return std::make_shared<ComputeNodePin>(owner, type, name/*, onChanged*/);
	}

	void ConnectTo(ComputeNodePinPtr pin);

	// void SetData(ComputeFramePtr frame);
	// void Notify(ComputeFramePtr frame);

	ComputeNodeImpl* Owner;
	PinType Type;
	std::string Name;
	Guid Id;

	// ComputeFramePtr Frame;
	// std::function<void(ComputeFramePtr)> OnChanged;
	std::vector<ComputeNodePinPtr> NextPins;

private:
	// std::vector<std::function<void(ComputeFramePtr)>> _functions;
};


//
struct Pin
{
	ComputeNodeImpl* Owner;
	std::string Name;
	Guid Id;

	virtual void ConnectFrom(Pin* pin) = 0;
};

struct InputPin : public Pin
{
	InputPin()
	{}
};


struct OutputPin : public Pin
{
	OutputPin()
	{}

	virtual void ConnectFrom(Pin* pin) override
	{}
};

} // End frox.