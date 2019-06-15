#pragma once

#include "Common.h"
#include "ComputeFrame.h"

namespace frox {

/**
 * @brief Compute flow class
 */
class FROX_API ComputeFlow
{
public:
	virtual ~ComputeFlow() {}

	// Nodes
	virtual ComputeNode* CreateNode(const char* type, const char* name = nullptr) = 0;
	virtual void DestoyNode(ComputeNode* node) = 0;

	template <typename ComputeNodeT>
	ComputeNodeT* CreateNode(const char* name = nullptr);

	// Connections
	virtual bool ConnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId) = 0;
	bool ConnectNodes(ComputeNode* outNode, ComputeNode* inNode, uint32_t inPinId = 0)
	{
		return ConnectNodes(outNode, 0, inNode, inPinId);
	}
	virtual bool DisconnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId) = 0;
	bool DisconnectNodes(ComputeNode* outNode, ComputeNode* inNode, uint32_t inPinId = 0)
	{
		return DisconnectNodes(outNode, 0, inNode, inPinId);
	}

	// Input/Output
	virtual uint32_t CreateEntry(const char* name = nullptr) = 0;
	virtual uint32_t CreateOutput(const char* name = nullptr) = 0;

	virtual void SetInput(uint32_t inId, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetOutput(uint32_t outId = 0) const = 0;

	virtual void ConnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) = 0;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) = 0;

	void SetInputScalar(uint32_t inId, uint8_t value);
	void SetInputScalar(uint32_t inId, uint16_t value);
	void SetInputScalar(uint32_t inId, uint32_t value);
	void SetInputScalar(uint32_t inId, float value);

	// Common
	virtual void Initialize() = 0;
	virtual bool WasInitialized() const = 0;
	virtual void Perform() = 0;
	virtual void Fetch() = 0;
	virtual uint32_t GetNumActiveTasks() const = 0;
};

template <typename ComputeNodeT>
ComputeNodeT* ComputeFlow::CreateNode(const char* name)
{
	ComputeNode* node = CreateNode(ComputeNodeT::GetNodeTypeStatic(), name);
	return reinterpret_cast<ComputeNodeT*>(node);
}

} // End frox