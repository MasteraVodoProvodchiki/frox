#include "AstraInspector.h"
#include "AstraDevice.h"
#include "AstraModule.h"

#include <Log.h>
#include <Frox.h>

namespace frox {

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
	// Start

	return _bStartedFlag;
}

void AstraInspector::Stop()
{
	try
	{
		_bStartedFlag = false;

		// Stop

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

bool AstraInspector::WaitReadFrame() const
{
	// Read

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

void AstraInspector::ProcessFrameset(void* frameset) const
{
	switch (_type)
	{
	case EInspectorType::Depth:
		ReadDepth(frameset);
		break;
	case EInspectorType::Color:
		ReadColor(frameset);
		break;
	case EInspectorType::Infrared:
		ReadInfrared(frameset);
		break;
	default:
		break;
	}
}

void AstraInspector::ReadDepth(void* frameset) const
{

}

void AstraInspector::ReadColor(void* frameset) const
{

}

void AstraInspector::ReadInfrared(void* frameset) const
{

}

} // End frox.
