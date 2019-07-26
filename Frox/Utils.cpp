#include "Utils.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#endif

namespace frox {
namespace utils {

#ifndef WITHOUT_OPENCV
void Copy(ComputeFramePtr src, ComputeFramePtr dst)
{
	assert(src->IsOpencv() && dst->IsOpencv());

	// Get cv::Mat
	OpenCVComputeFrameImpl* cvInput = reinterpret_cast<OpenCVComputeFrameImpl*>(src.get());
	OpenCVComputeFrameImpl* cvOuput = reinterpret_cast<OpenCVComputeFrameImpl*>(dst.get());

	cv::Mat inputMat = cvInput->GetMat();
	cv::Mat outputMat = cvOuput->GetMat();

	inputMat.copyTo(outputMat);
}
#else
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
#endif // WITHOUT_OPENCV

} // End utils
} // End frox