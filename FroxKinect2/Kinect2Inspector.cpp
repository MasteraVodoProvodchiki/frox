#include "Kinect2Inspector.h"
#include "Kinect2Device.h"
#include "Kinect2Module.h"
#include "Utils.h"

#include <Log.h>
#include <Frox.h>

#include <assert.h>

namespace frox {
namespace utils {

EKinect2StreamType InspectorTypeToStreamType(EInspectorType type)
{
	switch (type)
	{
	case EInspectorType::Depth:
		return EKinect2StreamType::STREAM_DEPTH;
	case EInspectorType::Color:
		return EKinect2StreamType::STREAM_COLOR;
	case EInspectorType::Infrared:
		return EKinect2StreamType::STREAM_INFRARED;
	default:
		assert(false);
	}

	return EKinect2StreamType::STREAM_ANY;
}

} // End utils

Kinect2Inspector::Kinect2Inspector(Kinect2DevicePtr device, EInspectorType type)
	: _device(device)
	, _type(type)
	, _bStartedFlag(false)
{}

Kinect2Inspector::~Kinect2Inspector()
{
	Stop();
}

bool Kinect2Inspector::Start()
{
	// Init Frame
	switch (_type)
	{
	case EInspectorType::Depth:
		_bStartedFlag = InitDepth();
		break;
	case EInspectorType::Color:
		_bStartedFlag = InitColor();
		break;
	case EInspectorType::Infrared:
		_bStartedFlag = InitInfrared();
		break;
	default:
		break;
	}

	// Start Reading
	_device->StartStream(utils::InspectorTypeToStreamType(_type));
	return _bStartedFlag;
}

void Kinect2Inspector::Stop()
{
	try
	{
		_bStartedFlag = false;

		// Stop Reading
		_device->StopStream(utils::InspectorTypeToStreamType(_type));

		_frame = nullptr;
	}
	catch (const std::exception& ex)
	{
		Log::Error("Kinect2Inspector::Stop exception: {0}", ex.what());
	}
}

bool Kinect2Inspector::IsValid() const
{
	return _bStartedFlag && _frame != nullptr;
}

ComputeFramePtr Kinect2Inspector::ReadFrame() const
{
	WaitReadFrame();
	return _frame;
}

bool Kinect2Inspector::InitDepth()
{
	assert(_device->GetKinectSensor() != nullptr);

	// Get Source
	IDepthFrameSource* pFrameSource = nullptr;
	HRESULT hr = _device->GetKinectSensor()->get_DepthFrameSource(&pFrameSource);
	if (FAILED(hr))
	{
		Log::Error("Kinect2Inspector depth source not valid.");
		return false;
	}

	_frame = CreateFrame(pFrameSource);
	return _frame != nullptr;
}

bool Kinect2Inspector::InitColor()
{
	assert(_device->GetKinectSensor() != nullptr);

	// Get Source
	IColorFrameSource* pFrameSource = nullptr;
	HRESULT hr = _device->GetKinectSensor()->get_ColorFrameSource(&pFrameSource);
	if (FAILED(hr))
	{
		Log::Error("Kinect2Inspector color source not valid.");
		return false;
	}

	// Create frame
	_frame = CreateFrame(pFrameSource);
	return _frame != nullptr;
}

bool Kinect2Inspector::InitInfrared()
{
	assert(_device->GetKinectSensor() != nullptr);

	// Get Source
	IInfraredFrameSource* pFrameSource = nullptr;
	HRESULT hr = _device->GetKinectSensor()->get_InfraredFrameSource(&pFrameSource);
	if (FAILED(hr))
	{
		Log::Error("Kinect2Inspector color source not valid.");
		return false;
	}

	// Create frame
	_frame = CreateFrame(pFrameSource);
	return _frame != nullptr;
}

bool Kinect2Inspector::WaitReadFrame() const
{
	auto frame = _device->WaitReadFrame(utils::InspectorTypeToStreamType(_type));
	if (!frame)
	{
		return false;
	}

	ProcessFrameset(frame);
	return true;
}

int32_t Kinect2Inspector::FindProfile(EKinect2StreamType streamType, EKinect2FormatType format, Kinect2StreamMode mode, Kinect2StreamProfile& out) const
{
	Kinect2StreamProfile profile;
	profile.StreamType = streamType;
	profile.Format = format;
	profile.Width = mode.Width;
	profile.Height = mode.Height;
	profile.Rate = mode.Rate;

	return _device->FindProfile(profile, out);
}

void Kinect2Inspector::EnsureProfileSupported(EKinect2StreamType streamType, EKinect2FormatType format, Kinect2StreamMode mode) const
{
	Kinect2StreamProfile profile;
	profile.StreamType = streamType;
	profile.Format = format;
	profile.Width = mode.Width;
	profile.Height = mode.Height;
	profile.Rate = mode.Rate;

	if (!_device->SupportsProfile(profile))
	{
		throw std::runtime_error("Profile not supported");
	}
}

void Kinect2Inspector::ProcessFrameset(ComputeFramePtr frame) const
{
	switch (_type)
	{
	case EInspectorType::Depth:
		ReadDepth(frame);
		break;
	case EInspectorType::Color:
		ReadColor(frame);
		break;
	case EInspectorType::Infrared:
		ReadInfrared(frame);
		break;
	default:
		break;
	}
}

void Kinect2Inspector::ReadDepth(ComputeFramePtr frame) const
{
	assert(_frame->GetSize() == frame->GetSize());
	assert(_frame->GetType() == frame->GetType());

	// TODO. Add copyTo
	Size size = _frame->GetSize();
	uint32_t elementSize = _frame->GetElementSize();
	for (uint32_t row = 0; row < size.Height; ++row)
	{
		memcpy(_frame->GetRowData(row), frame->GetRowData(row), elementSize * size.Width);	
	}
}

void Kinect2Inspector::ReadColor(ComputeFramePtr frame) const
{
	assert(_frame->GetSize() == frame->GetSize());
	assert(_frame->GetType() == frame->GetType());

	// TODO. Add copyTo
	Size size = _frame->GetSize();
	uint32_t elementSize = _frame->GetElementSize();
	for (uint32_t row = 0; row < size.Height; ++row)
	{
		memcpy(_frame->GetRowData(row), frame->GetRowData(row), elementSize * size.Width);
	}
}

void Kinect2Inspector::ReadInfrared(ComputeFramePtr frame) const
{
	assert(_frame->GetSize() == frame->GetSize());
	assert(_frame->GetType() == frame->GetType());

	// TODO. Add copyTo
	Size size = _frame->GetSize();
	uint32_t elementSize = _frame->GetElementSize();
	for (uint32_t row = 0; row < size.Height; ++row)
	{
		memcpy(_frame->GetRowData(row), frame->GetRowData(row), elementSize * size.Width);
	}
}

} // End frox.
