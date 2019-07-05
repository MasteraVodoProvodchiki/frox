#include "FrameSizeExpression.h"

namespace frox {

Variant FrameSizeExpression::GetValue(FlowDataImplPtr data) const
{
	ComputeFramePtr frame = *_frame.GetValue(data);
	return frame ? frame->GetSize() : Size({ 0, 0 });
}

} // End frox