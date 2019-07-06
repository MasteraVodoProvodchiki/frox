#pragma once

#include "ComputeNode.h"
#include "ComputeNodePin.h"
#include "ExpressionInput.h"
#include "Output.h"
#include "FlowDataImpl.h"

#include <vector>
#include <string>

namespace frox {

using ComputeNodeImplPtr = std::shared_ptr<class ComputeNodeImpl>;
using ComputeNodeImplWeak = std::weak_ptr<class ComputeNodeImpl>;
class ComputeNodeImpl
	: public ComputeNode
	, public std::enable_shared_from_this<ComputeNodeImpl>
{
	using Super = ComputeNode;

public:
	ComputeNodeImpl(const ComputeNodeInitializer& initializer);
	virtual ~ComputeNodeImpl() override;
	
	std::shared_ptr<ComputeNodeImpl> getptr()
	{
		return shared_from_this();
	}

	// ComputeNode overrides
	virtual const char* GetName() const override;
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData);
	
public:
	InputPin* GetInputPin(uint32_t inId = 0) const;
	OutputPin* GetOutputPin(uint32_t outId = 0) const;

	const std::vector<InputPin*>& GetInputsPins() const { return _inputs; }
	const std::vector<OutputPin*>& GetOutputPins() const { return _outputs; }

protected:
	void ReallocatePins();

	void RegisterInput(InputPin* input);
	void RegisterOutput(OutputPin* output);

private:
	std::string _name;

	std::vector<InputPin*> _inputs;
	std::vector<OutputPin*> _outputs;
};

} // End frox.