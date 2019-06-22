#pragma once

#include "ComputeNodeImpl.h"
#include "ComputeNodeFactory.h"
#include "ComputeFrame.h"

namespace frox {

class ComputeTask;
class CropComputeNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(CropComputeNode, "crop")
	using Super = ComputeNodeImpl;

public:
	CropComputeNode(const ComputeNodeInitializer& initializer);
	virtual ~CropComputeNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask() override;

	FROX_API void SetRect(Rect rect);

protected:
	// ComputeNodeImpl overrides
	virtual void OnPostInit() override;

private:
	uint32_t _input;
	uint32_t _output;

	Rect _rect;
};

} // End frox