#pragma once

#include "ComputeFlow.h"
#include "ComputeTask.h"

#include <vector>

namespace frox {

// Entry structs
struct ComputeFlowEntryNode
{
	ComputeNode* Node;
	uint32_t InId;
};

struct ComputeFlowEntry
{
	std::string Name;
	ComputeFramePtr Frame;
	std::vector<ComputeFlowEntryNode> Nodes;
};

// Output structs
struct ComputeFlowOutput
{
	std::string Name;
	ComputeFramePtr Frame;
};

/**
 * @brief Basic class of compute flow
 */
class ComputeFlowImpl : public ComputeFlow
{
	ComputeFlowImpl();

public:
	virtual ~ComputeFlowImpl() override;

	static ComputeFlowImpl* Create()
	{
		return new ComputeFlowImpl();
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
	// End ComputeFlow overrides

private:
	uint32_t GetNumActiveTasks() const;

private:
	bool _bWasInitialized;
	std::vector<ComputeNode*> _nodes;
	std::vector<ComputeTaskPtr> _tasks;

	std::vector<ComputeFlowEntry> _entries;
	std::vector<ComputeFlowOutput> _outputs;
};

} // End frox