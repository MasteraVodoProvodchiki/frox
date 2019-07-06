#pragma once

#include "ComputeFlowImpl.h"
#include "ComputeNodeImpl.h"
#include "ComputeTask.h"

#include <vector>
#include <mutex>

namespace frox {

/**
 * @brief Basic class of compute flow
 */
class BasicComputeFlowImpl : public ComputeFlowImpl
{
public:
	struct Connection
	{
		ComputeNodeImpl* OutNode;
		Pin* OutputPin;
		ComputeNodeImpl* InNode;
		Pin* InputPin;
	};

public:
	BasicComputeFlowImpl();
	virtual ~BasicComputeFlowImpl() override;

	static ComputeFlowImplPtr Create()
	{
		return std::make_shared<BasicComputeFlowImpl>();
	}

	// ComputeFlow overrides
	virtual ComputeNodeImpl* CreateNode(const char* type, const char* name = nullptr) override;
	virtual void DestoyNode(ComputeNodeImpl* node) override;
	virtual bool ConnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId) override;
	virtual bool DisconnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId) override;

	virtual uint32_t CreateEntry(const char* name, EPinValueType type = EPinValueType::Frame) override;
	virtual uint32_t CreateOutput(const char* name, EPinValueType type = EPinValueType::Frame) override;

	virtual uint32_t GetEntries(const ComputeFlowEntry** outEntries) const override;
	virtual uint32_t GetOutputs(const ComputeFlowOutput** outOutputs) const override;

	virtual int32_t FindEntryByName(const char* name) const override;
	virtual int32_t FindOutputByName(const char* name) const override;

	virtual void ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) override;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) override;

	virtual void ConnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId = 0) override;
	virtual void DisconnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId = 0) override;

	virtual void Prepare() override;
	virtual uint32_t GetNodes(const ComputeNodeImplPtr** outNodes) const override;
	// End ComputeFlow overrides
	
private:
	void DisconnectAll(ComputeNodeImpl* node);
	void MakeDirty();

private:
	bool _bDirty;

	std::vector<ComputeNodeImplPtr> _nodes;
	std::vector<ComputeNodeImplPtr> _sortedNodes;
	std::vector<Connection> _connections;

	std::vector<ComputeFlowEntry> _entries;
	std::vector<ComputeFlowOutput> _outputs;
};

} // End frox