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
	// virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	// virtual ComputeFramePtr GetOutput(uint32_t outId = 0) override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData);
	
	// virtual void Initialize() override;
	// virtual bool WasInitialized() const override;

	/*
	ComputeNodePinPtr GetInputPin(uint32_t inId = 0) const;
	ComputeNodePinPtr GetOutputPin(uint32_t outId = 0) const;
	*/

	InputPin* GetInputPin(uint32_t inId = 0) const;
	OutputPin* GetOutputPin(uint32_t outId = 0) const;

	/*
	const std::vector<ComputeNodePinPtr>& GetInputsPins() const { return _inputs; }
	const std::vector<ComputeNodePinPtr>& GetOutputPins() const { return _outputs; }
	*/
	const std::vector<InputPin*>& GetInputsPins() const { return _inputs; }
	const std::vector<OutputPin*>& GetOutputPins() const { return _outputs; }

protected:
	// virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame)
	// {}

	// virtual void OnPostInit()
	// {}

	void ReallocatePins();

	// uint32_t CreateInput(const char* name);
	// uint32_t CreateOutput(const char* name);
	void RegisterInput(InputPin* input);
	void RegisterOutput(OutputPin* output);

	// void SetOutput(uint32_t outId, ComputeFramePtr frame);

	// ComputeFramePtr GetInput(uint32_t inId = 0) const;
	// ComputeFramePtr GetOutput(uint32_t outId = 0) const;

private:
	std::string _name;
	// bool _bWasInitialized;

	// std::vector<ComputeNodePinPtr> _inputs;
	// std::vector<ComputeNodePinPtr> _outputs;
	std::vector<InputPin*> _inputs;
	std::vector<OutputPin*> _outputs;
};

} // End frox.