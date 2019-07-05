#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;

class OperationComputeNode : public ComputeNodeImpl
{
	using Super = ComputeNodeImpl;

protected:
	enum EType
	{
		ET_Add = 0,
		ET_Sub,
		ET_Mul,
		ET_Div
	};

public:
	OperationComputeNode(const ComputeNodeInitializer& initializer, EType type = ET_Add);
	virtual ~OperationComputeNode() override;
	
	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

protected:
	// ComputeNodeImpl overrides
	// virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame) override;

private:
	EType _type;

	TExpressionInput<ComputeFramePtr> _left;
	TExpressionInput<ComputeFramePtr> _right;
	TOutputFrame<ComputeFramePtr> _output;

	// uint32_t _left;
	// uint32_t _right;
	// uint32_t _output;
};


class AddComputeNode : public OperationComputeNode
{
	FROX_COMPUTENODE(AddComputeNode, "add")
	using Super = OperationComputeNode;

public:
	AddComputeNode(const ComputeNodeInitializer& initializer)
		: Super(initializer, ET_Add)
	{}
};

class SubComputeNode : public OperationComputeNode
{
	FROX_COMPUTENODE(SubComputeNode, "sub")
	using Super = OperationComputeNode;

public:
	SubComputeNode(const ComputeNodeInitializer& initializer)
		: Super(initializer, ET_Sub)
	{}
};

class MulComputeNode : public OperationComputeNode
{
	FROX_COMPUTENODE(MulComputeNode, "mul")
	using Super = OperationComputeNode;

public:
	MulComputeNode(const ComputeNodeInitializer& initializer)
		: Super(initializer, ET_Mul)
	{}
};

class DivComputeNode : public OperationComputeNode
{
	FROX_COMPUTENODE(DivComputeNode, "div")
	using Super = OperationComputeNode;

public:
	DivComputeNode(const ComputeNodeInitializer& initializer)
		: Super(initializer, ET_Div)
	{}
};

} // End frox