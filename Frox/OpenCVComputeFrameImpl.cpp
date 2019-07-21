#include "OpenCVComputeFrameImpl.h"
#include <assert.h>

namespace frox {

OpenCVComputeFrameImpl::OpenCVComputeFrameImpl(Size size, ComputeFrameType typeData, const void* data)
	: _type(typeData)
{	
	assert(size.Width > 0 && size.Height > 0);
	assert(typeData.Type != EComputeFrameType::ECFT_None && typeData.Channels > 0 && typeData.Channels <= 4);

	void* unconstData = const_cast<void*>(data);

	uint32_t channels = typeData.Channels;
	switch (typeData.Type)
	{
	case ECFT_Bool:
		_data = data != nullptr ? 
			cv::Mat(cv::Size(size.Width, size.Height), CV_8UC(channels), unconstData).clone() :
			cv::Mat(cv::Size(size.Width, size.Height), CV_8UC(channels));
		break;
	case ECFT_UInt8:
		_data = data != nullptr ?
			cv::Mat(cv::Size(size.Width, size.Height), CV_8UC(channels), unconstData).clone() :
			cv::Mat(cv::Size(size.Width, size.Height), CV_8UC(channels));
		break;
	case ECFT_UInt16:
		_data = data != nullptr ?
			cv::Mat(cv::Size(size.Width, size.Height), CV_16UC(channels), unconstData).clone() :
			cv::Mat(cv::Size(size.Width, size.Height), CV_16UC(channels));
		break;
	case ECFT_UInt32:
		_data = data != nullptr ?
			cv::Mat(cv::Size(size.Width, size.Height), CV_32SC(channels), unconstData).clone() :
			cv::Mat(cv::Size(size.Width, size.Height), CV_32SC(channels));
		break;
	case ECFT_Float:
		_data = data != nullptr ?
			cv::Mat(cv::Size(size.Width, size.Height), CV_32FC(channels), unconstData).clone() :
			cv::Mat(cv::Size(size.Width, size.Height), CV_32FC(channels));
		break;
	default:
		assert(false);
		break;
	}
}

OpenCVComputeFrameImpl::~OpenCVComputeFrameImpl()
{}

ComputeFrame::EClass OpenCVComputeFrameImpl::GetClassType() const
{
	return Opencv;
}

ComputeFrameType OpenCVComputeFrameImpl::GetType() const
{
	return _type;
}

uint32_t OpenCVComputeFrameImpl::GetChannels() const
{
	return _type.Channels;
}

uint32_t OpenCVComputeFrameImpl::GetElementSize() const
{
	return utils::FrameTypeToSize(_type.Type) * _type.Channels;
}

bool OpenCVComputeFrameImpl::IsValid() const
{
	return _data.size() != cv::Size(0, 0);
}

Size OpenCVComputeFrameImpl::GetSize() const
{
	cv::Size size = _data.size();
	return Size{
		uint32_t(size.width),
		uint32_t(size.height)
	};
}

const void* OpenCVComputeFrameImpl::GetData() const
{
	return _data.data;
}

const void* OpenCVComputeFrameImpl::GetRowData(uint32_t row) const
{
	cv::Size size = _data.size();
	assert(row < size.height);
	assert(_data.isContinuous());

	return reinterpret_cast<uint8_t*>(_data.data) + (row * size.height) * GetElementSize();
}

const void* OpenCVComputeFrameImpl::At(uint32_t row, uint32_t column) const
{
	cv::Size size = _data.size();
	assert(row < size.height && column < size.width);
	assert(_data.isContinuous());

	return reinterpret_cast<uint8_t*>(_data.data) + (row * size.width + column) * GetElementSize();
}

void* OpenCVComputeFrameImpl::GetData()
{
	return _data.data;
}

void* OpenCVComputeFrameImpl::GetRowData(uint32_t row)
{
	cv::Size size = _data.size();
	assert(row < size.height);
	assert(_data.isContinuous());

	return reinterpret_cast<uint8_t*>(_data.data) + (row * size.height) * GetElementSize();
}

void* OpenCVComputeFrameImpl::At(uint32_t row, uint32_t column)
{
	cv::Size size = _data.size();
	assert(row < size.height && column < size.width);
	assert(_data.isContinuous());

	return reinterpret_cast<uint8_t*>(_data.data) + (row * size.width + column) * GetElementSize();
}

} // End frox