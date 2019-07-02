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

	virtual uint32_t GetEntries(const ComputeFlowEntry** outEntries) const override;
	virtual uint32_t GetOutputs(const ComputeFlowOutput** outOutputs) const override;

	virtual int32_t FindEntryByName(const char* name) const override;
	virtual int32_t FindOutputByName(const char* name) const override;

	// virtual void SetInput(uint32_t inId, ComputeFramePtr frame) override;
	// virtual ComputeFramePtr GetOutput(uint32_t outId = 0) const override;

	virtual void ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) override;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) override;

	virtual void ConnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId = 0) override;
	virtual void DisconnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId = 0) override;

	virtual void Initialize() override;
	virtual bool WasInitialized() const override;

	virtual void Prepare() override;
	virtual uint32_t GetNodes(const ComputeNodeImplPtr** outNodes) const override;
	/*
	virtual void Perform() override;
	virtual void Fetch() override;
	virtual uint32_t GetNumActiveTasks() const override;
	*/
	// End ComputeFlow overrides
	
	// void SetOnPerformedCallback(std::function<void()> onPerformed);
	// void ClearOnPerformedCallback();

private:
	// void OnTaskCompleted(ComputeTask* task);
	// void Performed();
	void MakeDirty();

private:
	bool _bWasInitialized;
	bool _bDirty;

	std::vector<ComputeNodeImplPtr> _nodes;
	std::vector<ComputeNodeImplPtr> _sortedNodes;

	// mutable std::mutex _tasksMutex;
	// std::vector<ComputeTaskPtr> _tasks;

	// mutable std::mutex _onPerformedMutex;
	// std::function<void()> _onPerformed;

	std::vector<ComputeFlowEntry> _entries;
	std::vector<ComputeFlowOutput> _outputs;
};

} // End frox