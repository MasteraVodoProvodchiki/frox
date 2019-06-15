#pragma once

#include "Common.h"

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

	virtual void Initialize() = 0;
	virtual bool WasInitialized() const = 0;

	virtual void Perform() = 0;
	virtual void Fetch() = 0;
};

template <typename ComputeNodeT>
ComputeNodeT* ComputeFlow::CreateNode(const char* name)
{
	ComputeNode* node = CreateNode(ComputeNodeT::GetNodeTypeStatic(), name);
	return reinterpret_cast<ComputeNodeT*>(node);
}

} // End frox