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

protected:
	// ComputeNodeImpl overrides
	virtual void OnPostInit() override;

protected:
	uint32_t _width;
	uint32_t _height;
	EComputeFrameType _type;

	uint32_t _output;
};

class MakeFrameComputeNode : public MakeFrameBaseComputeNode
{
	FROX_COMPUTENODE(MakeFrameComputeNode, "makeframe")
	using Super = MakeFrameBaseComputeNode;

public:
	MakeFrameComputeNode(const ComputeNodeInitializer& initializer);

	// ComputeNode overrides
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask() override;

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
	virtual ComputeTask* CreateComputeTask() override;
};


} // End frox