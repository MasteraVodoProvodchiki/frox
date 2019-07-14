#include "RealSenseInspector.h"
#include "RealSenseDevice.h"
#include "RealSenseModule.h"

#include <Log.h>
#include <Frox.h>

namespace frox {

RealSenseInspector::RealSenseInspector(RealSenseDevicePtr device, EInspectorType type)
	: _device(device)
	, _type(type)
	, _bStartedFlag(false)
{}

RealSenseInspector::~RealSenseInspector()
{
	Stop();
}

bool RealSenseInspector::Start()
{
	try
	{
		if (_rsPipeline.get() || _bStartedFlag)
		{
			throw std::runtime_error("Already started");
		}

		rs2::context_ref rsContext(IRealSenseModule::Get().GetHandle());
		rs2::config rsConfig;

		rsConfig.enable_device(_device->GetSerial());

		ERealSenseStreamType type;
		ERealSenseFormatType format;
		rs2_stream rs2StreamType;
		rs2_format rs2Format;
		EComputeFrameType frameType;
		uint32_t frameChannels;

		switch (_type)
		{
		case EInspectorType::Depth:
			type = ERealSenseStreamType::STREAM_DEPTH;
			format = ERealSenseFormatType::FORMAT_Z16;
			rs2StreamType = RS2_STREAM_DEPTH;
			rs2Format = RS2_FORMAT_Z16;
			frameType = EComputeFrameType::ECFT_UInt16;
			frameChannels = 1;
			break;
		case EInspectorType::Color:
			type = ERealSenseStreamType::STREAM_COLOR;
			format = ERealSenseFormatType::FORMAT_RGBA8;
			rs2StreamType = RS2_STREAM_COLOR;
			rs2Format = RS2_FORMAT_RGBA8;
			frameChannels = 4;
			break;
		case EInspectorType::Infrared:
			type = ERealSenseStreamType::STREAM_INFRARED;
			format = ERealSenseFormatType::FORMAT_Y8;
			rs2StreamType = RS2_STREAM_INFRARED;
			rs2Format = RS2_FORMAT_Y8;
			frameChannels = 1;
			break;
		default:
			throw std::runtime_error("Unkown type");
		}

		RealSenseStreamMode config;
		EnsureProfileSupported(type, format, config);
		FindProfile(type, format, config, _currenProfile);

		rsConfig.enable_stream(rs2StreamType, config.Width, config.Height, rs2Format, config.Rate);

		_frame = FroxInstance()->CreateComputeFrame(Size{ uint32_t(config.Width), uint32_t(config.Height) }, ComputeFrameType{ frameType, frameChannels });

		// _rsPointCloud.reset(new rs2::pointcloud());
		// _rsPoints.reset(new rs2::points());


		_rsPipeline.reset(new rs2::pipeline());
		rs2::pipeline_profile rsProfile = _rsPipeline->start(rsConfig);

		_bStartedFlag = true;
	}
	catch (const rs2::error& ex)
	{
		Log::Error(
			"RealSenseSensor::Start exception : {0}(FUNC {1}; ARGS {2}; TYPE {3}"
			//,
			//ex.what(),
			//ex.get_failed_function(),
			//ex.get_failed_args(),
			//ex.get_type()
		);
		_bStartedFlag = false;
	}
	catch (const std::exception& ex)
	{
		Log::Error("RealSenseSensor::Start exception : {0}");
		// Log::Error("RealSenseSensor::Start exception : {0}", ex.what());
		_bStartedFlag = false;
	}

	if (!_bStartedFlag)
	{
		Stop();
	}

	return _bStartedFlag;
}

void RealSenseInspector::Stop()
{
	try
	{
		_bStartedFlag = false;

		if (_rsPipeline.get())
		{
			try {
				_rsPipeline->stop();
			}
			catch (...) {}
		}

		_rsPipeline.reset();
		//_rsAlign.reset();
		_rsPoints.reset();
		_rsPointCloud.reset();

		_frame = nullptr;
	}
	catch (const std::exception& ex)
	{
		Log::Error("RealSenseSensor::Stop exception: {0}", ex.what());
	}
}

bool RealSenseInspector::IsValid() const
{
	return _bStartedFlag && _frame != nullptr;
}

ComputeFramePtr RealSenseInspector::ReadFrame() const
{
	WaitReadFrame();
	return _frame;
}

bool RealSenseInspector::WaitReadFrame() const
{
	rs2::frameset Frameset;
	bool GotFrames;

	try
	{
		const float WaitFrameTimeout = 1.f;
		Frameset = _rsPipeline->wait_for_frames((unsigned int)(WaitFrameTimeout * 1000.0f));
		GotFrames = true;
	}
	catch (const rs2::error& ex)
	{
		Log::Error("FRealSenseSensor: wait_for_frames failed {0}", ex.what());
		GotFrames = false;
	}

	if (GotFrames)
	{
		ProcessFrameset(&Frameset);
	}

	return true;
}

int32_t RealSenseInspector::FindProfile(ERealSenseStreamType streamType, ERealSenseFormatType format, RealSenseStreamMode mode, RealSenseStreamProfile& out) const
{
	RealSenseStreamProfile profile;
	profile.StreamType = streamType;
	profile.Format = format;
	profile.Width = mode.Width;
	profile.Height = mode.Height;
	profile.Rate = mode.Rate;

	return _device->FindProfile(profile, out);
}

void RealSenseInspector::EnsureProfileSupported(ERealSenseStreamType streamType, ERealSenseFormatType format, RealSenseStreamMode mode) const
{
	RealSenseStreamProfile profile;
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

void RealSenseInspector::ProcessFrameset(rs2::frameset* frameset) const
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

void RealSenseInspector::ReadDepth(rs2::frameset* frameset) const
{
	rs2::depth_frame depthFrame = frameset->get_depth_frame();
	//(bAlignDepthToColor && RsAlign.Get()) ? RsAlign->process(*Frameset).get_depth_frame() : Frameset->get_depth_frame();

	// Get info
	const auto fw = depthFrame.get_width();
	const auto fh = depthFrame.get_height();
	const auto fbpp = depthFrame.get_bytes_per_pixel();
	const auto fs = depthFrame.get_stride_in_bytes();

	Size size = _frame->GetSize();
	// assert(size == Size{ fw, fh});

	uint32_t elementSize = _frame->GetElementSize();
	assert(elementSize == fbpp);

	const uint8_t* src = (const uint8_t*)depthFrame.get_data();

	for (int y = 0; y < fh; ++y)
	{
		void* dst = _frame->GetRowData(y);
		memcpy(dst, src, fw * fbpp);
		// dst += (fw * fbpp);
		src += fs;
	}
	/*
	if (_rsPointCloud.get())
	{
		*_rsPoints = _rsPointCloud->calculate(depthFrame);
	}
	*/
}

void RealSenseInspector::ReadColor(rs2::frameset* frameset) const
{
	rs2::video_frame colorFrame = frameset->get_color_frame();

	const auto fw = colorFrame.get_width();
	const auto fh = colorFrame.get_height();
	const auto fbpp = colorFrame.get_bytes_per_pixel();
	const auto fs = colorFrame.get_stride_in_bytes();

	// Copy
	Size size = _frame->GetSize();
	// assert(size == Size{ fw, fh});

	uint32_t elementSize = _frame->GetElementSize();
	// assert(elementSize == fbpp);

	const uint8_t* src = (const uint8_t*)colorFrame.get_data();

	for (int y = 0; y < fh; ++y)
	{
		void* dst = _frame->GetRowData(y);
		memcpy(dst, src, fw * fbpp);
		// dst += (fw * fbpp);
		src += fs;
	}

	// RsPointCloud->map_to(ColorFrame);
}

void RealSenseInspector::ReadInfrared(rs2::frameset* frameset) const
{
	rs2::video_frame infraredFrame = frameset->get_infrared_frame();

	const auto fw = infraredFrame.get_width();
	const auto fh = infraredFrame.get_height();
	const auto fbpp = infraredFrame.get_bytes_per_pixel();
	const auto fs = infraredFrame.get_stride_in_bytes();

	// Copy
	Size size = _frame->GetSize();
	// assert(size == Size{ fw, fh});

	uint32_t elementSize = _frame->GetElementSize();
	// assert(elementSize == fbpp);

	const uint8_t* src = (const uint8_t*)infraredFrame.get_data();

	for (int y = 0; y < fh; ++y)
	{
		void* dst = _frame->GetRowData(y);
		memcpy(dst, src, fw * fbpp);
		// dst += (fw * fbpp);
		src += fs;
	}
}

/*
void FRealSenseInspector::GetDepthCameraIntrinsics(Vector2f& focalLength, Vector2f& principalPoint, Vector3f& radialDistortion) const
{
	const rs2_intrinsics& intrinsics = _depthProfile.Intrinsics;

	focalLength = Vector2f(intrinsics.fx, intrinsics.fy);
	principalPoint = Vector2f(intrinsics.ppx, intrinsics.ppy);
	// intrinsics.model
	// radialDistortion = Vector3f(intrinsics.RadialDistortionFourthOrder, cameraIntrinsics.RadialDistortionSecondOrder, cameraIntrinsics.RadialDistortionSixthOrder);
}

float FRealSenseInspector::GetDepthValue(int32 x, int32 y) const
{
	if (_depthFrame && x >= 0 && y >= 0 && x < _depthResoluation.X && y < _depthResoluation.Y)
	{
		FrameGuardRead guard(_depthFrame);
		auto data = guard.AsUint16();
		if (data != nullptr)
		{
			uint32 index = y * _depthResoluation.X + x;
			return data[index];
		}
	}

	return 0.f;
}

Vector3f FRealSenseInspector::MapCameraPointToDepthSpace(const Vector3f& cameraPoint) const
{
	const rs2_intrinsics& intrinsics = _depthProfile.Intrinsics;

	Vector2f pixel;
	rs2_project_point_to_pixel((float*)(&pixel.X), &intrinsics, (const float*)(&cameraPoint.X));

	const float depthVal = cameraPoint.Z; // / 1000.0f;
	return Vector3f(
		pixel.X,
		pixel.Y,
		depthVal
	);
}

Vector3f FRealSenseInspector::MapDepthPointToCameraSpace(const Vector3f& depthPoint) const
{
	const rs2_intrinsics& intrinsics = _depthProfile.Intrinsics;

	// float depthInSm = depthPoint.Z * 0.1f;
	Vector3f cameraPoint;
	rs2_deproject_pixel_to_point((float*)(&cameraPoint.X), &intrinsics, (const float*)(&depthPoint.X), depthPoint.Z);

	return cameraPoint * Vector3f(0.001f);
}

Vector2f FRealSenseInspector::MapDepthPointToColorSpace(const Vector3f& point) const
{
	Log::Warning("FRealSenseInspector::MapDepthPointToColorSpace unsupported");
	return Vector2f();
}

Vector3f FRealSenseInspector::MapColorPointToCameraSpace(const Vector3f& point) const
{
	Log::Warning("FRealSenseInspector::MapColorPointToCameraSpace unsupported");
	return Vector3f();
}
*/

} // End frox.
