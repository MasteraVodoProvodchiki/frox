#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "Variant.h"

namespace frox {

class ComputeTask;
class MakeFrameBaseComputeNode : public ComputeNodeImpl
{
	using Super = ComputeNodeImpl;

public:
	MakeFrameBaseComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~MakeFrameBaseComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;

	FROX_API void SetWidth(uint32_t width);
	FROX_API void SetHeight(uint32_t height);
	FROX_API void SetType(EComputeFrameType type);
	FROX_API void SetChannels(uint32_t channels);

protected:
	// ComputeNodeImpl overrides
	// virtual void OnPostInit() override;

protected:
	TExpressionInput<uint32_t> _width;
	TExpressionInput<uint32_t> _height;
	TExpressionInput<uint32_t> _channels;

	// uint32_t _width;
	// uint32_t _height;
	EComputeFrameType _type;
	// uint32_t _channels;

	
	TOutputFrame<ComputeFramePtr> _output;
	// uint32_t _output;
};

class MakeFrameComputeNode : public MakeFrameBaseComputeNode
{
	FROX_COMPUTENODE(MakeFrameComputeNode, "makeframe")
	using Super = MakeFrameBaseComputeNode;

public:
	MakeFrameComputeNode(const ComputeNodeInitializer& initializer);

	// ComputeNode overrides
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetValue(Variant value);

private:
	Variant _value;
};

class MakeZeroFrameComputeNode : public MakeFrameComputeNode
{
	FROX_COMPUTENODE(MakeZeroFrameComputeNode, "makezeroframe")
	using Super = MakeFrameComputeNode;

public:
	MakeZeroFrameComputeNode(const ComputeNodeInitializer& initializer);
};

class MakeNoiseFrameComputeNode : public MakeFrameBaseComputeNode
{
	FROX_COMPUTENODE(MakeNoiseFrameComputeNode, "makenoiseframe")
	using Super = MakeFrameBaseComputeNode;

public:
	MakeNoiseFrameComputeNode(const ComputeNodeInitializer& initializer);

	// ComputeNode overrides
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;
};

class ConstFrameComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(ConstFrameComputeNode, "constframe")
	using Super = ComputeNodeImpl;

public:
	ConstFrameComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~ConstFrameComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROX_API void SetFrame(ComputeFramePtr frame);

protected:
	// ComputeNodeImpl overrides
	// virtual void OnPostInit() override;

private:
	ComputeFramePtr _frame;
	TOutput<ComputeFramePtr> _output;
	// uint32_t _output;
};


} // End frox