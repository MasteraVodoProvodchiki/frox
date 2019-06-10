#pragma once

#include "ComputeNode.h"
#include "ComputeNodePin.h"

#include <vector>
#include <string.h>

namespace frox {

class ComputeNodeImpl : public ComputeNode
{
	using Super = ComputeNode;

public:
	ComputeNodeImpl(const ComputeNodeInitializer& initializer);
	virtual ~ComputeNodeImpl() override;
	
	// ComputeNode overrides
	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) override;

	ComputeNodePinPtr GetInputPin(uint32_t inId = 0) const;
	ComputeNodePinPtr GetOutputPin(uint32_t outId = 0) const;

protected:
	virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame)
	{}

	virtual void OnPostInit()
	{}

	uint32_t CreateInput(const char* name);
	uint32_t CreateOutput(const char* name);

	void SetOutput(uint32_t outId, ComputeFramePtr frame);

	ComputeFramePtr GetInput(uint32_t inId = 0) const;
	ComputeFramePtr GetOutput(uint32_t outId = 0) const;

private:
	std::vector<ComputeNodePinPtr> _inputs;
	std::vector<ComputeNodePinPtr> _outputs;
};

} // End frox.