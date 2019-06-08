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
	virtual ComputeTask* CreateComputeTask() override;

protected:
	// ComputeNodeImpl overrides
	virtual void OnInputChanged(uint32_t inId, ComputeFramePtr frame) override;

private:
	EType _type;
	uint32_t _left;
	uint32_t _right;
	uint32_t _output;
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

class MakeFrameComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(MakeFrameComputeNode, "makeframe")
	using Super = ComputeNodeImpl;

public:
	MakeFrameComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~MakeFrameComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask() override;

	void SetWidth(uint32_t width)
	{
		_width = width;
	}

	void SetHeight(uint32_t height)
	{
		_height = height;
	}

	void SetValue(float value)
	{
		_value = value;
	}

protected:
	// ComputeNodeImpl overrides
	virtual void OnPostInit() override;

private:
	uint32_t _width;
	uint32_t _height;
	float _value;

	uint32_t _output;
};

class MakeZeroFrameComputeNode : public MakeFrameComputeNode
{
	FROX_COMPUTENODE(MakeZeroFrameComputeNode, "makezeroframe")
	using Super = MakeFrameComputeNode;

public:
	MakeZeroFrameComputeNode(const ComputeNodeInitializer& initializer);
};


} // End frox