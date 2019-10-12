#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class ShowFrameBaseNode : public ComputeNodeImpl
{
public:
	ShowFrameBaseNode(const ComputeNodeInitializer& initializer)
		: ComputeNodeImpl(initializer)
	{}

	FROX_API void SetName(const std::string& name);
	const char* GetName() const
	{
		return _name.c_str();
	}

private:
	std::string _name;
};

class ShowFrameNode : public ShowFrameBaseNode
{
	FROX_COMPUTENODE(ShowFrameNode, "showframe")
	using Super = ShowFrameBaseNode;

public:
	ShowFrameNode(const ComputeNodeInitializer& initializer);
	virtual ~ShowFrameNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<bool> _show;
};

class ShowFrameWithContoursNode : public ShowFrameBaseNode
{
	FROX_COMPUTENODE(ShowFrameWithContoursNode, "showframewithcontours")
	using Super = ShowFrameBaseNode;

public:
	ShowFrameWithContoursNode(const ComputeNodeInitializer& initializer);
	virtual ~ShowFrameWithContoursNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

private:
	TExpressionInput<ComputeFramePtr> _input;
	TExpressionInput<ComputeDataPtr> _contours;
	TExpressionInput<ComputeDataPtr> _hierarchy;
	TExpressionInput<bool> _show;
};

} // End frox