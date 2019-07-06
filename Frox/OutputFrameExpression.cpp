#include "OutputFrameExpression.h"

namespace frox {


Variant OutputFrameExpression::GetValue(FlowDataImplPtr data) const
{
	return data->GetValue(_pinId);
}

ComputeFramePtr OutputFrameExpression::GetFrame(FlowDataImplPtr data) const
{
	return data->GetFrame(_pinId);
}

} // End frox