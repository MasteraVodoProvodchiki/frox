#pragma once

#include "ComputeFlow.h"

#include <vector>

namespace frox {

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
	virtual bool ConnectNodes(ComputeNode* outNode, int outPinId, ComputeNode* inNode, int inPinId) override;
	virtual bool DisconnectNodes(ComputeNode* outNode, int outPinId, ComputeNode* inNode, int inPinId) override;

	virtual void Perform() override;
	virtual void Fetch() override;

private:
	std::vector<ComputeNode*> _nodes;
};

} // End frox