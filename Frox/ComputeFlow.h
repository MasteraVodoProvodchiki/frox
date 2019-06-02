#pragma once

#include "Common.h"

namespace frox {

class ComputeFlow
{
public:
	virtual ~ComputeFlow() {}

	virtual ComputeNode* CreateNode(const char* type, const char* name = nullptr) = 0;
	virtual void DestoyNode(ComputeNode* node) = 0;

	template <typename ComputeNodeT>
	ComputeNodeT* CreateNode(const char* name = nullptr)
	{
		ComputeNode* node = CreateNode(ComputeNodeT::GetNodeTypeStatic(), name);
		return reinterpret_cast<ComputeNodeT*>(node);
	}

	//
	virtual bool ConnectNodes(ComputeNode* outNode, int outPinId, ComputeNode* inNode, int inPinId) = 0;
	bool ConnectNodes(ComputeNode* outNode, ComputeNode* inNode, int inPinId = 0)
	{
		return ConnectNodes(outNode, 0, inNode, inPinId);
	}
	virtual bool DisconnectNodes(ComputeNode* outNode, int outPinId, ComputeNode* inNode, int inPinId) = 0;
	bool DisconnectNodes(ComputeNode* outNode, ComputeNode* inNode, int inPinId = 0)
	{
		return DisconnectNodes(outNode, 0, inNode, inPinId);
	}

	virtual void Perform() = 0;
	virtual void Fetch() = 0;
};

} // End frox