#pragma once

#include <ComputeFrame.h>
#include "Native.h"

namespace frox {

ComputeFramePtr CreateDepthFrame(IFrameDescription* pFrameDescription);
ComputeFramePtr CreateColorFrame(IFrameDescription* pFrameDescription);
ComputeFramePtr CreateInfraredFrame(IFrameDescription* pFrameDescription);

ComputeFramePtr CreateFrame(IDepthFrameSource* pFrameSource);
ComputeFramePtr CreateFrame(IColorFrameSource* pFrameSource);
ComputeFramePtr CreateFrame(IInfraredFrameSource* pFrameSource);

} // End frox.