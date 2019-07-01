#include "FrameSizeExpression.h"

namespace frox {

Variant FrameSizeExpression::GetValue() const
{
	ComputeFramePtr frame = _frame.GetValue();
	return frame ? frame->GetSize() : Size({ 0, 0 });
}

} // End frox