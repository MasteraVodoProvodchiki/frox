#include "AstraInspector.h"
#include "AstraDevice.h"
#include "AstraModule.h"
#include "AstraUtils.h"

#include <Log.h>
#include <Frox.h>

namespace frox {
namespace utils {

EAstraStreamType InspectorTypeToStreamType(EInspectorType type)
{
	switch (type)
	{
	case EInspectorType::Depth:
		return EAstraStreamType::STREAM_DEPTH;
	case EInspectorType::Color:
		return EAstraStreamType::STREAM_COLOR;
	case EInspectorType::Infrared:
		return EAstraStreamType::STREAM_INFRARED;
	default:
		assert(false);
	}

	return EAstraStreamType::STREAM_ANY;
}

} // End utils

AstraInspector::AstraInspector(AstraDevicePtr device, EInspectorType type)
	: _device(device)
	, _type(type)
	, _bStartedFlag(false)
{}

AstraInspector::~AstraInspector()
{
	Stop();
}

bool AstraInspector::Start()
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

void AstraInspector::Stop()
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
		Log::Error("AstraInspector::Stop exception: {0}", ex.what());
	}
}

bool AstraInspector::IsValid() const
{
	return _bStartedFlag && _frame != nullptr;
}

ComputeFramePtr AstraInspector::ReadFrame() const
{
	WaitReadFrame();
	return _frame;
}

bool AstraInspector::InitDepth()
{
	// Get stream
	astra::StreamReader& reader = _device->GetReader();
	auto depthStream = reader.stream<astra::DepthStream>();
	if (!depthStream.is_available())
	{
		return false;
	}

	// Create frame
	_frame = CreateFrame(depthStream);
	return _frame != nullptr;
}

bool AstraInspector::InitColor()
{
	// Get stream
	astra::StreamReader& reader = _device->GetReader();
	auto colorStream = reader.stream<astra::ColorStream>();
	if (!colorStream.is_available())
	{
		return false;
	}

	// Create frame
	_frame = CreateFrame(colorStream);
	return _frame != nullptr;
}

bool AstraInspector::InitInfrared()
{
	// Get stream
	astra::StreamReader& reader = _device->GetReader();
	auto infraredStream = reader.stream<astra::InfraredStream>();
	if (!infraredStream.is_available())
	{
		return false;
	}

	// Create frame
	_frame = CreateFrame(infraredStream);
	return _frame != nullptr;
}

bool AstraInspector::WaitReadFrame() const
{
	auto frame = _device->WaitReadFrame(utils::InspectorTypeToStreamType(_type));
	if (!frame)
	{
		return false;
	}

	ProcessFrameset(frame);
	return true;
}

int32_t AstraInspector::FindProfile(EAstraStreamType streamType, EAstraFormatType format, AstraStreamMode mode, AstraStreamProfile& out) const
{
	AstraStreamProfile profile;
	profile.StreamType = streamType;
	profile.Format = format;
	profile.Width = mode.Width;
	profile.Height = mode.Height;
	profile.Rate = mode.Rate;

	return _device->FindProfile(profile, out);
}

void AstraInspector::EnsureProfileSupported(EAstraStreamType streamType, EAstraFormatType format, AstraStreamMode mode) const
{
	AstraStreamProfile profile;
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

void AstraInspector::ProcessFrameset(ComputeFramePtr frame) const
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

void AstraInspector::ReadDepth(ComputeFramePtr frame) const
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

void AstraInspector::ReadColor(ComputeFramePtr frame) const
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

void AstraInspector::ReadInfrared(ComputeFramePtr frame) const
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
