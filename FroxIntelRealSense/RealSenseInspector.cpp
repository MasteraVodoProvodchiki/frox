#include "RealSenseInspector.h"
#include "RealSenseDevice.h"
#include "RealSenseModule.h"

#include <Log.h>

namespace frox {

RealSenseInspector::RealSenseInspector(RealSenseDevice* device)
	: _device(device)
	, _bStartedFlag(false)
{}

RealSenseInspector::~RealSenseInspector()
{}

bool RealSenseInspector::Start()
{
	try
	{
		if (_rsPipeline.get() || _bStartedFlag)
		{
			throw std::runtime_error("Already started");
		}

		rs2::context_ref rsContext(RealSenseModule::Get().GetHandle());
		rs2::config rsConfig;

		rsConfig.enable_device(_device->GetSerial());

		/*
		// Depth
		RealSenseStreamMode depthConfig;
		EnsureProfileSupported(ERealSenseStreamType::STREAM_DEPTH, ERealSenseFormatType::FORMAT_Z16, depthConfig);
		FindProfile(ERealSenseStreamType::STREAM_DEPTH, ERealSenseFormatType::FORMAT_Z16, depthConfig, _currenProfile);

		rsConfig.enable_stream(RS2_STREAM_DEPTH, depthConfig.Width, depthConfig.Height, RS2_FORMAT_Z16, depthConfig.Rate);
		// RsAlign.Reset(new rs2::align(RS2_STREAM_COLOR));
		_depthResoluation = Vector2i(depthConfig.Width, depthConfig.Height);
		_depthPixelSize = sizeof(ushort);

		_depthFrame = new Frame("Sensor0Depth");
		_depthFrame->Create(_depthResoluation.x, _depthResoluation.y, DataType<ushort>::type);


		// Color
		FRealSenseStreamMode colorConfig;
		EnsureProfileSupported(ERealSenseStreamType::STREAM_COLOR, ERealSenseFormatType::FORMAT_RGBA8, colorConfig);
		FindProfile(ERealSenseStreamType::STREAM_COLOR, ERealSenseFormatType::FORMAT_RGBA8, colorConfig, _colorProfile);

		rsConfig.enable_stream(RS2_STREAM_COLOR, colorConfig.Width, colorConfig.Height, RS2_FORMAT_RGBA8, colorConfig.Rate);

		_colorResoluation = Vector2i(colorConfig.Width, colorConfig.Height);

		_colorFrame = new Frame("Sensor0Color");
		_colorFrame->Create(_colorResoluation.x, _colorResoluation.y, CV_8UC4);
		// _colorYuvData = Mat(ColorConfigHeight + ColorConfigHeight / 2, ColorConfigWidth, CV_8UC1);

		// _colorMapToCamera.resize(ColorConfigHeight * ColorConfigHeight);


		// Infrared
		// rsConfig.enable_stream(RS2_STREAM_INFRARED, InfraredConfig.Width, InfraredConfig.Height, RS2_FORMAT_Y8, InfraredConfig.Rate);
		*/
		//
		_rsPointCloud.reset(new rs2::pointcloud());
		_rsPoints.reset(new rs2::points());


		//
		_rsPipeline.reset(new rs2::pipeline());
		rs2::pipeline_profile rsProfile = _rsPipeline->start(rsConfig);

		_bStartedFlag = true;
	}
	catch (const rs2::error& ex)
	{
		Log::Error(
			"RealSenseSensor::Start exception : {0}(FUNC {1}; ARGS {2}; TYPE {3}",
			ex.what(),
			ex.get_failed_function(),
			ex.get_failed_args(),
			ex.get_type()
		);
		_bStartedFlag = false;
	}
	catch (const std::exception& ex)
	{
		Log::Error("RealSenseSensor::Start exception : {0}", ex.what());
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
	ReadDepth(frameset);
	ReadColor(frameset);
}

void RealSenseInspector::ReadDepth(rs2::frameset* frameset) const
{
	rs2::depth_frame depthFrame = frameset->get_depth_frame();
	//(bAlignDepthToColor && RsAlign.Get()) ? RsAlign->process(*Frameset).get_depth_frame() : Frameset->get_depth_frame();

	/*
	// Get info
	const auto fw = depthFrame.get_width();
	const auto fh = depthFrame.get_height();
	const auto fbpp = depthFrame.get_bytes_per_pixel();
	const auto fs = depthFrame.get_stride_in_bytes();

	if (_depthResoluation.X != fw || _depthResoluation.Y != fh || sizeof(ushort) != fbpp)
	{
		Log::Error("Invalid video_frame: {0} Width={1} Height={2} Bpp={3}", "DepthFrame", fw, fh, fbpp);
		return;
	}


	// Copy
	FrameGuard guard(_depthFrame);
	auto dst = guard.AsByte();
	const uint8_t* src = (const uint8_t*)depthFrame.get_data();

	for (int y = 0; y < fh; ++y)
	{
		memcpy(dst, src, fw * fbpp);
		dst += (fw * fbpp);
		src += fs;
	}

	if (_rsPointCloud.get())
	{
		*_rsPoints = _rsPointCloud->calculate(depthFrame);
	}
	*/
}

void RealSenseInspector::ReadColor(rs2::frameset* frameset) const
{
	rs2::video_frame colorFrame = frameset->get_color_frame();

	/*
	const auto fw = colorFrame.get_width();
	const auto fh = colorFrame.get_height();
	const auto fbpp = colorFrame.get_bytes_per_pixel();
	const auto fs = colorFrame.get_stride_in_bytes();

	if (_colorResoluation.X != fw || _colorResoluation.Y != fh) // || sizeof() != fbpp)
	{
		Log::Error("Invalid video_frame: {0} Width={1} Height={2} Bpp={3}", "DepthFrame", fw, fh, fbpp);
		return;
	}


	// Copy
	FrameGuard guard(_colorFrame);
	auto dst = guard.AsByte();
	const uint8_t* src = (const uint8_t*)colorFrame.get_data();

	for (int y = 0; y < fh; ++y)
	{
		memcpy(dst, src, fw * fbpp);
		dst += (fw * fbpp);
		src += fs;
	}

	// RsPointCloud->map_to(ColorFrame);
	*/
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
