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
	FROX_API ComputeNodeImpl(const ComputeNodeInitializer& initializer);
	FROX_API virtual ~ComputeNodeImpl() override;
	
	std::shared_ptr<ComputeNodeImpl> getptr()
	{
		return shared_from_this();
	}

	// ComputeNode overrides
	FROX_API virtual const char* GetName() const override;
	FROX_API virtual void AllocateDefaultPins() override;
	FROX_API virtual bool IsValid() const override;
	FROX_API virtual int32_t FindInputByName(const char* pinName) const override;
	FROX_API virtual int32_t FindOutputByName(const char* pinName) const override;
	FROX_API virtual Guid FindInputGuidByName(const char* pinName) const override;
	FROX_API virtual Guid FindOutputGuidByName(const char* pinName) const override;

	FROX_API virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData);
	
public:
	FROX_API InputPin* GetInputPin(uint32_t inId = 0) const;
	FROX_API OutputPin* GetOutputPin(uint32_t outId = 0) const;

	const std::vector<InputPin*>& GetInputsPins() const { return _inputs; }
	const std::vector<OutputPin*>& GetOutputPins() const { return _outputs; }

protected:
	FROX_API void ReallocatePins();

	FROX_API void RegisterInput(InputPin* input);
	FROX_API void RegisterOutput(OutputPin* output);

private:
	std::string _name;

	std::vector<InputPin*> _inputs;
	std::vector<OutputPin*> _outputs;
};

} // End frox.