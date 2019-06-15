#pragma once

#include "ComputeFlow.h"
#include "BasicComputeFlowImpl.h"

namespace frox {


/**
 * @brief Basic class of compute flow
 */
class BasicComputeFlow : public ComputeFlow
{
	BasicComputeFlow();

public:
	virtual ~BasicComputeFlow() override;

	static BasicComputeFlow* Create()
	{
		return new BasicComputeFlow();
	}

	// ComputeFlow overrides
	virtual ComputeNode* CreateNode(const char* type, const char* name = nullptr) override;
	virtual void DestoyNode(ComputeNode* node) override;
	virtual bool ConnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId) override;
	virtual bool DisconnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId) override;

	virtual uint32_t CreateEntry(const char* name = nullptr) override;
	virtual uint32_t CreateOutput(const char* name = nullptr) override;

	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) const override;

	virtual void ConnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) override;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) override;

	virtual void Initialize() override;
	virtual bool WasInitialized() const override;
	virtual void Perform() override;
	virtual void Fetch() override;
	virtual uint32_t GetNumActiveTasks() const override;
	// End ComputeFlow overrides
	
private:
	ComputeFlowImplPtr _impl;
};

} // End frox