#include "ExpressionInput.h"
#include "OutputFrameExpression.h"

namespace frox {

void ExpressionInput::ConnectFrom(Pin* pin)
{
	Expr = OutputFrameExpression::Create(pin->Id);
}

void ExpressionInput::ConnectFrom(Guid id)
{
	Expr = OutputFrameExpression::Create(id);
}

} // End frox