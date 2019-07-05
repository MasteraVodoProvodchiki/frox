#pragma once

#include "Expression.h"
#include "ExpressionInput.h"
#include "ComputeFrame.h"

namespace frox {

class FrameSizeExpression : public Expression
{
public:
	FrameSizeExpression(ExpressionPtr expr)
		: _frame(expr)
	{}

	FrameSizeExpression(ComputeFramePtr frame)
		: _frame(frame)
	{}

	virtual ~FrameSizeExpression()
	{}

	virtual Variant GetValue(FlowDataImplPtr data) const override;

private:
	TExpressionInput<ComputeFramePtr> _frame;
};

} // End frox