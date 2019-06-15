#pragma once

#include "ComputeFlowImpl.h"
#include "ComputeTask.h"

#include <vector>
#include <mutex>

namespace frox {

// Entry structs
struct ComputeFlowEntryNode
{
	ComputeNodeImpl* Node;
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
class BasicComputeFlowImpl : public ComputeFlowImpl
{
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

	virtual uint32_t CreateEntry(const char* name = nullptr) override;
	virtual uint32_t CreateOutput(const char* name = nullptr) override;

	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) const override;

	virtual void ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) override;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) override;

	virtual void Initialize() override;
	virtual bool WasInitialized() const override;
	virtual void Perform() override;
	virtual void Fetch() override;
	virtual uint32_t GetNumActiveTasks() const override;
	// End ComputeFlow overrides
	
	void SetOnPerformedCallback(std::function<void()> onPerformed);
	void ClearOnPerformedCallback();

private:
	void OnTaskCompleted(ComputeTask* task);
	void Performed();

private:
	bool _bWasInitialized;
	std::vector<ComputeNodeImpl*> _nodes;

	mutable std::mutex _tasksMutex;
	std::vector<ComputeTaskPtr> _tasks;

	mutable std::mutex _onPerformedMutex;
	std::function<void()> _onPerformed;

	std::vector<ComputeFlowEntry> _entries;
	std::vector<ComputeFlowOutput> _outputs;
};

} // End frox