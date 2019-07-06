#include "OutputFrameExpression.h"

namespace frox {

ComputeFramePtr OutputFrameExpression::GetFrame(FlowDataImplPtr data) const
{
	// ComputeNodeImplPtr node = _nodeWeak.lock();
	//return node ? node->GetOutput(_pinId) : nullptr;
	ComputeFramePtr frame = data->GetFrame(_pinId);
	return frame;
}

} // End frox