#pragma once

#include "ComputeNode.h"
#include "ComputeNodePin.h"

#include <vector>
#ifdef _WIN32
#include <string.h>
#else
#include <string>
#endif

namespace frox {

class ComputeNodeImpl : public ComputeNode
{
	using Super = ComputeNode;

public:
	ComputeNodeImpl(const ComputeNodeInitializer& initializer);
	virtual ~ComputeNodeImpl() override;
	
	// ComputeNode overrides
	virtual const char* GetName() const override;
	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) override;
	virtual void Initialize() override;
	virtual bool WasInitialized() const override;

	ComputeNodePinPtr GetInputPin(uint32_t inId = 0) const;
	ComputeNodePinPtr GetOutputPin(uint32_t outId = 0) const;

	const std::vector<ComputeNodePinPtr>& GetInputsPins() const { return _inputs; }
	const std::vector<ComputeNodePinPtr>& GetOutputPins() const { return _outputs; }

protected:
	virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame)
	{}

	virtual void OnPostInit()
	{}

	void ReallocatePins();

	uint32_t CreateInput(const char* name);
	uint32_t CreateOutput(const char* name);

	void SetOutput(uint32_t outId, ComputeFramePtr frame);

	ComputeFramePtr GetInput(uint32_t inId = 0) const;
	ComputeFramePtr GetOutput(uint32_t outId = 0) const;

private:
	std::string _name;
	bool _bWasInitialized;
	std::vector<ComputeNodePinPtr> _inputs;
	std::vector<ComputeNodePinPtr> _outputs;
};

} // End frox.