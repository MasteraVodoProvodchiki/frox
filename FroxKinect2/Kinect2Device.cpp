#include "Kinect2Device.h"
#include "Kinect2Inspector.h"
#include "Utils.h"
#include <Log.h>

#include <assert.h>

namespace frox {

/*
class MultiFrameListener : public astra::FrameListener
{
public:
	MultiFrameListener(Kinect2Device* device)
		: _device(device)
	{}

	static MultiFrameListenerPtr Create(Kinect2Device* device)
	{
		return std::make_shared<MultiFrameListener>(device);
	}

	// FrameListener override
	virtual void on_frame_ready(astra::StreamReader& reader, astra::Frame& frame) override
	{
		// Used sync method
		_device->UpdateDepth(frame);
		_device->UpdateColor(frame);
		_device->UpdateInfrared(frame);
	}

private:
	Kinect2Device* _device;
};


astra::DepthStream ConfigureDepth(astra::StreamReader& reader)
{
	auto depthStream = reader.stream<astra::DepthStream>();

	auto oldMode = depthStream.mode();

	astra::ImageStreamMode depthMode;
	depthMode.set_width(640);
	depthMode.set_height(480);
	depthMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_DEPTH_MM);
	depthMode.set_fps(30);
	depthStream.set_mode(depthMode);

	auto newMode = depthStream.mode();
	// Log::Info("Changed depth mode: {0}x{1} @ {2} -> {3}x{4} @ {5}",
	//	oldMode.width(), oldMode.height(), oldMode.fps(),
	//	newMode.width(), newMode.height(), newMode.fps());

	return depthStream;
}


astra::ColorStream ConfigureColor(astra::StreamReader& reader)
{
	auto colorStream = reader.stream<astra::ColorStream>();

	auto oldMode = colorStream.mode();

	astra::ImageStreamMode colorMode;
	colorMode.set_width(640);
	colorMode.set_height(480);
	colorMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_RGB888);
	colorMode.set_fps(30);
	colorStream.set_mode(colorMode);

	auto newMode = colorStream.mode();
	// Log::Info("Changed color mode: {0}x{1} @ {2} -> {3}x{4} @ {5}",
	//	oldMode.width(), oldMode.height(), oldMode.fps(),
	//	newMode.width(), newMode.height(), newMode.fps());

	return colorStream;
}

astra::InfraredStream ConfigureInfrared(astra::StreamReader& reader)
{
	auto infraredStream = reader.stream<astra::InfraredStream>();

	auto oldMode = infraredStream.mode();

	astra::ImageStreamMode infraredMode;
	infraredMode.set_width(640);
	infraredMode.set_height(480);
	infraredMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_GRAY16);
	infraredMode.set_fps(30);
	infraredStream.set_mode(infraredMode);

	auto newMode = infraredStream.mode();
	// Log::Info("Changed color mode: {0}x{1} @ {2} -> {3}x{4} @ {5}",
	//	oldMode.width(), oldMode.height(), oldMode.fps(),
	//	newMode.width(), newMode.height(), newMode.fps());

	return infraredStream;
}
*/

Kinect2Device::Kinect2Device(IKinectSensor* kinectSensor)
	: _kinectSensor(kinectSensor)
	, _depthFrameReader(nullptr)
	, _colorFrameReader(nullptr)
	, _infraredFrameReader(nullptr)
{}

Kinect2Device::~Kinect2Device()
{
	_sensors.clear();

	// close the Sensor
	if (_depthFrameReader != nullptr)
	{
		_depthFrameReader->Release();
	}

	if (_colorFrameReader != nullptr)
	{
		_colorFrameReader->Release();
	}

	if (_infraredFrameReader != nullptr)
	{
		_infraredFrameReader->Release();
	}
}

SensorInspectorPtr Kinect2Device::CreateInpector(EInspectorType type)
{
	auto inspector = Kinect2Inspector::Create(this->GetPtr(), type);
	inspector->Start();
	return inspector;
}

void Kinect2Device::QueryData()
{
	assert(_kinectSensor != nullptr);

	_sensors.clear();

	// Read sensors	
	// Initialize the Kinect and get the depth reader
	// IMultiSourceFrameReader* pMultiSourceFrameReader = nullptr;

	HRESULT hr = _kinectSensor->Open();
	if (FAILED(hr))
	{
		Log::Error("Kinect2Sensor didn't opened.");
		return;
	}

	for (auto sensor : _sensors)
	{
		sensor->QueryData();
	}

	InitFrames();
}

bool Kinect2Device::FindProfile(Kinect2StreamProfile profile, Kinect2StreamProfile& out) const
{
	for (auto sensor : _sensors)
	{
		if (sensor->FindProfile(profile, out))
		{
			return true;
		}
	}

	return false;
}

bool Kinect2Device::SupportsProfile(Kinect2StreamProfile Profile) const
{
	for (auto sensor : _sensors)
	{
		if (sensor->SupportsProfile(Profile))
		{
			return true;
		}
	}

	return false;
}

void Kinect2Device::InitFrames()
{
	// Depth
	{
		IDepthFrameSource* pFrameSource = nullptr;
		HRESULT hr = _kinectSensor->get_DepthFrameSource(&pFrameSource);
		if (FAILED(hr))
		{
			Log::Error("Kinect2Device depth source not valid.");
			return;
		}

		hr = pFrameSource->OpenReader(&_depthFrameReader);
		if (FAILED(hr))
		{
			Log::Error("Kinect2Device depth reader not valid.");
			return;
		}
		_depthFrameReader->put_IsPaused(TRUE);

		IFrameDescription* pFrameDescription = nullptr;
		pFrameSource->get_FrameDescription(&pFrameDescription);
		if (pFrameDescription != nullptr)
		{
			_depthFrame = CreateDepthFrame(pFrameDescription);
			pFrameDescription->Release();
		}
		else
		{
			Log::Error("Kinect2Device get depth FrameDescription failed.");
		}

		pFrameSource->Release();
	}

	// Color
	{
		IColorFrameSource* pFrameSource = nullptr;
		HRESULT hr = _kinectSensor->get_ColorFrameSource(&pFrameSource);
		if (FAILED(hr))
		{
			Log::Error("Kinect2Device color source not valid.");
			return;
		}

		hr = pFrameSource->OpenReader(&_colorFrameReader);
		if (FAILED(hr))
		{
			Log::Error("Kinect2Device color reader not valid.");
			return;
		}
		_colorFrameReader->put_IsPaused(TRUE);

		IFrameDescription* pFrameDescription = nullptr;
		pFrameSource->get_FrameDescription(&pFrameDescription);
		if (pFrameDescription != nullptr)
		{
			_colorFrame = CreateColorFrame(pFrameDescription);
			pFrameDescription->Release();
		}
		else
		{
			Log::Error("Kinect2Device get color FrameDescription failed.");
		}

		pFrameSource->Release();
	}

	// Infrared
	{
		IInfraredFrameSource* pFrameSource = nullptr;
		HRESULT hr = _kinectSensor->get_InfraredFrameSource(&pFrameSource);
		if (FAILED(hr))
		{
			Log::Error("Kinect2Device infrared source not valid.");
			return;
		}

		hr = pFrameSource->OpenReader(&_infraredFrameReader);
		if (FAILED(hr))
		{
			Log::Error("Kinect2Device infrared reader not valid.");
			return;
		}
		_infraredFrameReader->put_IsPaused(TRUE);

		IFrameDescription* pFrameDescription = nullptr;
		pFrameSource->get_FrameDescription(&pFrameDescription);
		if (pFrameDescription != nullptr)
		{
			_infraredFrame = CreateInfraredFrame(pFrameDescription);
			pFrameDescription->Release();
		}
		else
		{
			Log::Error("Kinect2Device get infrared FrameDescription failed.");
		}

		pFrameSource->Release();
	}
}

void Kinect2Device::StartStream(EKinect2StreamType streamType)
{
	switch (streamType)
	{
	case EKinect2StreamType::STREAM_ANY:
		StartDepthStream();
		StartColorStream();
		StartInfraredStream();
		break;
	case EKinect2StreamType::STREAM_DEPTH:
		StartDepthStream();
		break;
	case EKinect2StreamType::STREAM_COLOR:
		StartColorStream();
		break;
	case EKinect2StreamType::STREAM_INFRARED:
		StartInfraredStream();
		break;
	}
}

void Kinect2Device::StopStream(EKinect2StreamType streamType)
{
	switch (streamType)
	{
	case EKinect2StreamType::STREAM_ANY:
		StopDepthStream();
		StopColorStream();
		StopInfraredStream();
		break;
	case EKinect2StreamType::STREAM_DEPTH:
		StopDepthStream();
		break;
	case EKinect2StreamType::STREAM_COLOR:
		StopColorStream();
		break;
	case EKinect2StreamType::STREAM_INFRARED:
		StopInfraredStream();
		break;
	}
}

void Kinect2Device::StartDepthStream()
{
	if (_depthStart == 0)
	{
		_depthFrameReader->put_IsPaused(FALSE);
	}

	_depthStart++;
}

void Kinect2Device::StartColorStream()
{
	if (_colorStart == 0)
	{
		_colorFrameReader->put_IsPaused(FALSE);
	}

	_colorStart++;
}

void Kinect2Device::StartInfraredStream()
{
	if (_infraredStart == 0)
	{
		_infraredFrameReader->put_IsPaused(FALSE);
	}

	_infraredStart++;
}

void Kinect2Device::StopDepthStream()
{
	assert(_depthStart > 0);
	_depthStart--;

	if (_depthStart == 0)
	{
		_depthFrameReader->put_IsPaused(TRUE);
	}
}

void Kinect2Device::StopColorStream()
{
	assert(_colorStart > 0);
	_colorStart--;

	if (_colorStart == 0)
	{
		_colorFrameReader->put_IsPaused(TRUE);
	}
}

void Kinect2Device::StopInfraredStream()
{
	assert(_infraredStart > 0);
	_infraredStart--;


	if (_infraredStart == 0)
	{
		_infraredFrameReader->put_IsPaused(TRUE);
	}
}

void Kinect2Device::UpdateDepth()
{
	assert(_depthFrame != nullptr && _depthFrameReader != nullptr);

	IDepthFrame* pDepthFrame = nullptr;
	IFrameDescription* pDepthFrameDescription = nullptr;

	// Read frame
	HRESULT hr = _depthFrameReader->AcquireLatestFrame(&pDepthFrame);
	if (FAILED(hr))
	{
		return;
	}

	// Read frame data
	int64_t nTime = 0;
	int nWidth = 0, nHeight = 0;
	USHORT nDepthMinReliableDistance = 0, nDepthMaxDistance = 0;
	uint32_t nBufferSize = 0;
	uint16_t *pBuffer = nullptr;

	hr = pDepthFrame->get_RelativeTime(&nTime);
	if (SUCCEEDED(hr))
	{
		hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
	}

	if (SUCCEEDED(hr))
	{
		hr = pDepthFrameDescription->get_Width(&nWidth);
	}

	if (SUCCEEDED(hr))
	{
		hr = pDepthFrameDescription->get_Height(&nHeight);
	}

	if (SUCCEEDED(hr))
	{
		hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
	}

	if (SUCCEEDED(hr))
	{
		// In order to see the full range of depth (including the less reliable far field depth)
		// we are setting nDepthMaxDistance to the extreme potential depth threshold
		nDepthMaxDistance = USHRT_MAX;

		// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
		//// hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
	}

	if (SUCCEEDED(hr))
	{
		hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
	}
	// End read frame info

	if (SUCCEEDED(hr))
	{
		//_depthFrame->GetSize() == Size{ uint32_t(nWidth), uint32_t(nHeight) }
		//assert();
		assert(_depthFrame->GetElementSize() == sizeof(uint16_t));

		auto dst = _depthFrame->GetData<uint16_t>();
		memcpy(dst, pBuffer, nWidth * nHeight * sizeof(uint16_t));
	}

	// Release descriptions.
	if (pDepthFrameDescription != nullptr)
	{
		pDepthFrameDescription->Release();
	}

	// Release frames.
	if (pDepthFrame != nullptr)
	{
		pDepthFrame->Release();
	}
}

void Kinect2Device::UpdateColor()
{
	assert(_colorFrame != nullptr);

	IColorFrame* pColorFrame = nullptr;
	IFrameDescription* pColorFrameDescription = nullptr;

	// Read frame
	HRESULT hr = _colorFrameReader->AcquireLatestFrame(&pColorFrame);
	if (FAILED(hr))
	{
		return;
	}

	// Read frame info
	int64_t nTime = 0;
	int nWidth = 0, nHeight = 0;
	uint32_t nBufferSize = 0;
	BYTE *pBuffer = nullptr;

	ColorImageFormat imageFormat;
	pColorFrame->get_RawColorImageFormat(&imageFormat);

	//assert(format == ColorImageFormat::ColorImageFormat_Bgra);
	//assert(format == ColorImageFormat::ColorImageFormat_Yuv || format == ColorImageFormat::ColorImageFormat_Yuy2);

	hr = pColorFrame->get_RelativeTime(&nTime);	
	if (SUCCEEDED(hr))
	{
		hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
	}

	if (SUCCEEDED(hr))
	{
		hr = pColorFrameDescription->get_Width(&nWidth);
	}

	if (SUCCEEDED(hr))
	{
		hr = pColorFrameDescription->get_Height(&nHeight);
	}

	if (SUCCEEDED(hr))
	{
		hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, &pBuffer);
	}
	// End read frame info

	// Read data
	if (SUCCEEDED(hr))
	{
		auto dst = _depthFrame->GetData<uint16_t>();
		auto frameSize = _depthFrame->GetSize();

		if (imageFormat == ColorImageFormat_Bgra)
		{
			hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
			memcpy(dst, pBuffer, nBufferSize);
		}
		else
		{
			assert(_depthFrame->GetElementSize() == sizeof(uint16_t) * 4);
			
			nBufferSize = frameSize.Width * frameSize.Height * sizeof(uint8_t) * 4;
			hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(dst), ColorImageFormat::ColorImageFormat_Bgra);
		}

	}

	// Release descriptions.
	if (pColorFrameDescription != nullptr)
	{
		pColorFrameDescription->Release();
	}

	// Release frames.
	if (pColorFrame != nullptr)
	{
		pColorFrame->Release();
	}

}

void Kinect2Device::UpdateInfrared()
{
	assert(_infraredFrame != nullptr);

	IInfraredFrame* pInfraredFrame = nullptr;
	IFrameDescription* pInfraredFrameDescription = nullptr;

	// Read frame
	HRESULT hr = _infraredFrameReader->AcquireLatestFrame(&pInfraredFrame);
	if (FAILED(hr))
	{
		return;
	}

	// Read frame info
	int64_t nTime = 0;
	int nWidth = 0, nHeight = 0;
	uint32_t nBufferSize = 0;
	uint16_t *pBuffer = nullptr;

	hr = pInfraredFrame->get_RelativeTime(&nTime);
	if (SUCCEEDED(hr))
	{
		hr = pInfraredFrame->get_FrameDescription(&pInfraredFrameDescription);
	}

	if (SUCCEEDED(hr))
	{
		hr = pInfraredFrameDescription->get_Width(&nWidth);
	}

	if (SUCCEEDED(hr))
	{
		hr = pInfraredFrameDescription->get_Height(&nHeight);
	}

	if (SUCCEEDED(hr))
	{
		hr = pInfraredFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
	}

	if (SUCCEEDED(hr))
	{
		assert(_infraredFrame->GetElementSize() == sizeof(uint16_t));

	}
	// End read frame info

	// Read data
	if (SUCCEEDED(hr))
	{
		auto dst = _depthFrame->GetData<uint16_t>();
		memcpy(dst, pBuffer, nBufferSize);
	}

	// Release descriptions.
	if (pInfraredFrameDescription != nullptr)
	{
		pInfraredFrameDescription->Release();
	}

	// Release frames.
	if (pInfraredFrame != nullptr)
	{
		pInfraredFrame->Release();
	}
}

ComputeFramePtr Kinect2Device::WaitReadFrame(EKinect2StreamType streamType)
{
	// Get Last frame
	switch (streamType)
	{
	case EKinect2StreamType::STREAM_DEPTH:
		UpdateDepth();
		return _depthFrame;
	case EKinect2StreamType::STREAM_COLOR:
		UpdateColor();
		return _colorFrame;
	case EKinect2StreamType::STREAM_INFRARED:
		UpdateInfrared();
		return _infraredFrame;
	default:
		break;
	}

	return nullptr;
}

Kinect2SensorPtr Kinect2Device::NewSensor(void* handle, const char* name)
{
	Kinect2SensorPtr sensor = Kinect2Sensor::Create(handle);
	return sensor;
}

} // End frox.
