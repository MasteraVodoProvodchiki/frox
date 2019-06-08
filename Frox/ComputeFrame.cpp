#include "ComputeFrame.h"

namespace frox {
namespace utils {

uint32_t FrameTypeToSize(EComputeFrameType type)
{
	switch (type)
	{
	case ECFT_UInt8:
		return sizeof(uint8_t);
	case ECFT_Float:
		return sizeof(float);
	default:
		return 0;
	}
}

} // End utils
} // End frox