#include "ExpressionInput.h"
#include "OutputFrameExpression.h"

namespace frox {

void ExpressionInput::ConnectFrom(Pin* pin)
{
	Expr = OutputFrameExpression::Create(pin->Id);
}

} // End frox