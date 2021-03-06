#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "ComputeNodeImpl.h"
#include "Guid.h"

#include <memory>
#include <string>
#include <vector>

namespace frox {

// Entry structs
struct ComputeFlowEntryNode
{
	ComputeNodeImpl* Node;
	Guid PinId; 
};

struct ComputeFlowEntry
{
	std::string Name;
	EPinValueType Type;
	std::vector<ComputeFlowEntryNode> Nodes;
};

// Output structs
struct ComputeFlowOutput
{
	std::string Name;
	EPinValueType Type;
	std::vector<ComputeFlowEntryNode> Nodes;
};


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
	virtual uint32_t CreateEntry(const char* name, EPinValueType type = EPinValueType::Frame) = 0;
	virtual uint32_t CreateOutput(const char* name, EPinValueType type = EPinValueType::Frame) = 0;

	/**
	 * @brief 
	 * @param outEntries output enties poineter
	 * @param count
	 */
	virtual uint32_t GetEntries(const ComputeFlowEntry** outEntries) const = 0;

	/**
	 * @brief
	 * @param outOutputs output outputs poineter
	 * @param count
	 */
	virtual uint32_t GetOutputs(const ComputeFlowOutput** outOutputs) const = 0;

	virtual int32_t FindEntryByName(const char* name) const = 0;
	virtual int32_t FindOutputByName(const char* name) const = 0;

	virtual void ConnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) = 0;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNodeImpl* inNode, uint32_t inPinId = 0) = 0;

	virtual void ConnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId = 0) = 0;
	virtual void DisconnectOutput(uint32_t outputId, ComputeNodeImpl* outNode, uint32_t outPinId = 0) = 0;

	// Common
	virtual void Prepare() = 0;
	virtual uint32_t GetNodes(const ComputeNodeImplPtr** outNodes) const = 0;
};
using ComputeFlowImplPtr = std::shared_ptr<ComputeFlowImpl>;

} // End frox