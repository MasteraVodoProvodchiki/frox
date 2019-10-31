#pragma once

#include <ComputeFrame.h>
#include "AstraNative.h"

namespace frox {

ComputeFramePtr CreateFrame(astra::DepthStream depthStream);
ComputeFramePtr CreateFrame(astra::ColorStream colorStream);
ComputeFramePtr CreateFrame(astra::InfraredStream infraredStream);

} // End frox.