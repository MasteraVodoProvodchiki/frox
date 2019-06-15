#pragma once

#include "Common.h"
#include "ComputeFrame.h"

#include <memory>

namespace frox {

class ComputeNodeImpl;

/**
 * @brief Compute flow class impl
 */
class ComputeFlowImpl : public std::enable_shared_from_this<ComputeFlowImpl>
{
public:
	virtual ~ComputeFlowImpl() {}

	std::shared_ptr<ComputeFlowImpl> GetPtr()
	{
		return shared_from_this();
	}

	// Nodes
	virtual ComputeNodeImpl* CreateNode(const char* type, const char* name = nullptr) = 0;
	virtual void DestoyNode(ComputeNodeImpl* node) = 0;

	// Connections
	virtual bool ConnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId) = 0;
	virtual bool DisconnectNodes(ComputeNodeImpl* outNode, uint32_t outPinId, ComputeNodeImpl* inNode, uint32_t inPinId) = 0;

	// Input/Output
	virtual uint32_t CreateEntry(const char* name = nullptr) = 0;
	virtual uint32_t CreateOutput(const char* name = nullptr) = 0;

	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) const = 0;

	virtual void ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) = 0;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) = 0;

	// Common
	virtual void Initialize() = 0;
	virtual bool WasInitialized() const = 0;
	virtual void Perform() = 0;
	virtual void Fetch() = 0;
	virtual uint32_t GetNumActiveTasks() const = 0;
};
using ComputeFlowImplPtr = std::shared_ptr<ComputeFlowImpl>;

} // End frox