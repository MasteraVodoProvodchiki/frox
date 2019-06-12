#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"
#include "Variant.h"

namespace frox {

class ComputeTask;
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

	FROX_API void SetWidth(uint32_t width);
	FROX_API void SetHeight(uint32_t height);
	FROX_API void SetType(EComputeFrameType type);
	FROX_API void SetValue(Variant value);

protected:
	// ComputeNodeImpl overrides
	virtual void OnPostInit() override;

private:
	uint32_t _width;
	uint32_t _height;
	EComputeFrameType _type;
	Variant _value;

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