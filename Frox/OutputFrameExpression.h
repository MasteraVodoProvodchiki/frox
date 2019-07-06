#pragma once

#include "Expression.h"
#include "ComputeNodeImpl.h"

namespace frox {

class OutputFrameExpression : public Expression
{
public:
	OutputFrameExpression(Guid pinId)
		: _pinId(pinId)
	{}

	virtual ~OutputFrameExpression()
	{}

	static ExpressionPtr Create(Guid pinId)
	{
		return std::make_shared<OutputFrameExpression>(pinId);
	}

	// Expression overrides
	virtual Variant GetValue(FlowDataImplPtr data) const override;
	virtual ComputeFramePtr GetFrame(FlowDataImplPtr data) const override;

private:
	Guid _pinId;
};

} // End frox