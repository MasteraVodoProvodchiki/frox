#include "Utils.h"

namespace frox {
namespace utils {

void Copy(ComputeFramePtr src, ComputeFramePtr dst)
{
	assert(src->GetSize() == dst->GetSize());
	assert(src->GetType() == dst->GetType());

	Size size = src->GetSize();
	uint32_t elementSize = src->GetElementSize();
	for (uint32_t row = 0; row < size.Height; ++row)
	{
		const void* srcRow = src->GetRowData(row);
		void* dstRow = dst->GetRowData(row);

		memcpy(dstRow, srcRow, size.Width * elementSize);
	}
}

} // End utils
} // End frox