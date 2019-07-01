#pragma once

#include "Expression.h"
#include "ComputeNodeImpl.h"

namespace frox {

class OutputFrameExpression : public Expression
{
public:
	OutputFrameExpression(ComputeNodeImplPtr node, uint32_t pinId = 0)
		: _nodeWeak(node)
		, _pinId(pinId)
	{}

	virtual ~OutputFrameExpression()
	{}

	virtual Type GetType() const override { return ET_Frame; }
	virtual ComputeFramePtr GetFrame() const override;

private:
	ComputeNodeImplWeak _nodeWeak;
	uint32_t _pinId;
};

} // End frox