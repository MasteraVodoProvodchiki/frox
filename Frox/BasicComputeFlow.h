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

	virtual uint32_t CreateEntry(const char* name, EPinValueType type = EPinValueType::Frame) override;
	virtual uint32_t CreateOutput(const char* name, EPinValueType type = EPinValueType::Frame) override;

	virtual int32_t FindEntryByName(const char* name) const override;
	virtual int32_t FindOutputByName(const char* name) const override;

	virtual void ConnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) override;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) override;

	virtual void ConnectOutput(uint32_t outputId, ComputeNode* outNode, uint32_t outPinId = 0) override;
	virtual void DisconnectOutput(uint32_t outputId, ComputeNode* outNode, uint32_t outPinId = 0) override;

	ComputeFlowImplPtr GetComputeFlow() const { return _impl; }

private:
	ComputeFlowImplPtr _impl;
};

} // End frox