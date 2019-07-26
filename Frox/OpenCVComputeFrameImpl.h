#pragma once

#include "ComputeFrame.h"
#include "OpenCV.h"

namespace frox {

/**
 * OpenCV ComputeFrame with cv::Mat
 */
class OpenCVComputeFrameImpl : public ComputeFrame
{
public:
	OpenCVComputeFrameImpl(Size size = Size{ 0, 0 }, ComputeFrameType type = ComputeFrameType{ ECFT_None, 0 }, const void* data = nullptr);
	virtual ~OpenCVComputeFrameImpl() override;

	static ComputeFramePtr Create(Size size = Size{ 0, 0 }, ComputeFrameType type = ComputeFrameType{ ECFT_None, 0 }, const void* data = nullptr)
	{
		return std::make_shared<OpenCVComputeFrameImpl>(size, type, data);
	}

	// ComputeFrame overrides
	virtual EClass GetClassType() const override;
	virtual ComputeFrameType GetType() const override;
	virtual uint32_t GetChannels() const override;
	virtual uint32_t GetElementSize() const override;
	virtual bool IsValid() const override;
	virtual Size GetSize() const override;
	virtual const void* GetData() const override;
	virtual const void* GetRowData(uint32_t row) const override;
	virtual const void* At(uint32_t row, uint32_t column) const override;
	virtual void* GetData() override;
	virtual void* GetRowData(uint32_t row) override;
	virtual void* At(uint32_t row, uint32_t column) override;

	// Inlines
	cv::Mat GetMat() const
	{
		return _data;
	}

private:
	ComputeFrameType _type;
	cv::Mat _data;
};

} // End frox