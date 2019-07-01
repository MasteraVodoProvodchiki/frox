#include "OutputFrameExpression.h"

namespace frox {

ComputeFramePtr OutputFrameExpression::GetFrame() const
{
	ComputeNodeImplPtr node = _nodeWeak.lock();
	return node ? node->GetOutput(_pinId) : nullptr;
}

} // End frox