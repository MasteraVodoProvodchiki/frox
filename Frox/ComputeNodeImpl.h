#pragma once

#include "ComputeNode.h"

#include <vector>
#include <string.h>

namespace frox {

struct ComputeNodePin
{
	std::string Name;
	ComputeFramePtr Frame;
};

class ComputeNodeImpl : public ComputeNode
{
	using Super = ComputeNode;

public:
	ComputeNodeImpl(const ComputeNodeInitializer& initializer);
	virtual ~ComputeNodeImpl() override;
	
	// ComputeNode overrides
	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) override;

protected:
	virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame)
	{}

	uint32_t CreateInput(const char* name);
	uint32_t CreateOutput(const char* name);

	void SetOutput(uint32_t outId, ComputeFramePtr frame);
	ComputeFramePtr GetInput(uint32_t inId = 0) const;
	ComputeFramePtr GetOutput(uint32_t outId = 0) const;

private:
	std::vector<ComputeNodePin> _inputs;
	std::vector<ComputeNodePin> _outputs;
};

} // End frox.