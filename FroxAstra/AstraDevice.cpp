#include "AstraDevice.h"
#include "AstraInspector.h"
#include "AstraUtils.h"

namespace frox {

class MultiFrameListener : public astra::FrameListener
{
public:
	MultiFrameListener(AstraDevice* device)
		: _device(device)
	{}

	static MultiFrameListenerPtr Create(AstraDevice* device)
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
	AstraDevice* _device;
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

AstraDevice::AstraDevice(astra::StreamSet streamSet)
	: _streamSet(streamSet)
{}

AstraDevice::~AstraDevice()
{
	_sensors.clear();

	// close the Sensor
}

SensorInspectorPtr AstraDevice::CreateInpector(EInspectorType type)
{
	auto inspector = AstraInspector::Create(this->GetPtr(), type);
	inspector->Start();
	return inspector;
}

bool AstraDevice::MapDepthFrameToColorFrame(ComputeFramePtr depthFrame, ComputeFramePtr colorFrame, ComputeFramePtr output)
{
	//TODO: implement mapping
	return false;
}

void AstraDevice::QueryData()
{
	_sensors.clear();

	// Read sensors	
	ConfigureDepth(_reader);
	ConfigureColor(_reader);
	ConfigureInfrared(_reader);

	for (auto sensor : _sensors)
	{
		sensor->QueryData();
	}

	InitFrames();

	// Add listerner
	_listerner = MultiFrameListener::Create(this);
	_reader.add_listener(*_listerner);
}

bool AstraDevice::FindProfile(AstraStreamProfile profile, AstraStreamProfile& out) const
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

bool AstraDevice::SupportsProfile(AstraStreamProfile Profile) const
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

void AstraDevice::InitFrames()
{
	auto depthStream = _reader.stream<astra::DepthStream>();
	if (depthStream.is_available())
	{
		_depthFrame = CreateFrame(depthStream);
	}

	auto colorStream = _reader.stream<astra::ColorStream>();
	if (colorStream.is_available())
	{
		_colorFrame = CreateFrame(colorStream);
	}

	auto infraredStream = _reader.stream<astra::InfraredStream>();
	if (infraredStream.is_available())
	{
		_infraredFrame = CreateFrame(infraredStream);
	}
}

void AstraDevice::StartStream(EAstraStreamType streamType)
{
	switch (streamType)
	{
	case EAstraStreamType::STREAM_ANY:
		StartDepthStream();
		StartColorStream();
		StartInfraredStream();
		break;
	case EAstraStreamType::STREAM_DEPTH:
		StartDepthStream();
		break;
	case EAstraStreamType::STREAM_COLOR:
		StartColorStream();
		break;
	case EAstraStreamType::STREAM_INFRARED:
		StartInfraredStream();
		break;
	}
}

void AstraDevice::StopStream(EAstraStreamType streamType)
{
	switch (streamType)
	{
	case EAstraStreamType::STREAM_ANY:
		StopDepthStream();
		StopColorStream();
		StopInfraredStream();
		break;
	case EAstraStreamType::STREAM_DEPTH:
		StopDepthStream();
		break;
	case EAstraStreamType::STREAM_COLOR:
		StopColorStream();
		break;
	case EAstraStreamType::STREAM_INFRARED:
		StopInfraredStream();
		break;
	}
}

void AstraDevice::StartDepthStream()
{
	if (_depthStart == 0)
	{
		auto depthStream = _reader.stream<astra::DepthStream>();
		if (depthStream.is_available())
		{
			depthStream.start();
		}
	}

	_depthStart++;
}

void AstraDevice::StartColorStream()
{
	if (_colorStart == 0)
	{
		auto colorStream = _reader.stream<astra::ColorStream>();
		if (colorStream.is_available())
		{
			colorStream.start();
		}
	}

	_colorStart++;
}

void AstraDevice::StartInfraredStream()
{
	if (_infraredStart == 0)
	{
		auto infraredStream = _reader.stream<astra::InfraredStream>();
		if (infraredStream.is_available())
		{
			infraredStream.stop();
		}
	}

	_infraredStart++;
}

void AstraDevice::StopDepthStream()
{
	assert(_depthStart > 0);
	_depthStart--;

	if (_depthStart == 0)
	{
		auto depthStream = _reader.stream<astra::DepthStream>();
		if (depthStream.is_available())
		{
			depthStream.stop();
		}
	}
}

void AstraDevice::StopColorStream()
{
	assert(_colorStart > 0);
	_colorStart--;

	if (_colorStart == 0)
	{
		auto colorStream = _reader.stream<astra::ColorStream>();
		if (colorStream.is_available())
		{
			colorStream.stop();
		}
	}
}

void AstraDevice::StopInfraredStream()
{
	assert(_infraredStart > 0);
	_infraredStart--;


	if (_infraredStart == 0)
	{
		auto infraredStream = _reader.stream<astra::InfraredStream>();
		if (infraredStream.is_available())
		{
			infraredStream.stop();
		}
	}
}

void AstraDevice::UpdateDepth(astra::Frame& frame)
{
	assert(_depthFrame != nullptr);

	const astra::PointFrame pointFrame = frame.get<astra::PointFrame>();
	if (!pointFrame.is_valid())
	{
		// depthView_.texture_.update(&depthView_.buffer_[0]);
		return;
	}

	const int width = pointFrame.width();
	const int height = pointFrame.height();
	const astra::Vector3f* src = pointFrame.data();
	assert(_depthFrame->GetElementSize() == sizeof(uint16_t));

	for (int row = 0; row < height; row++)
	{
		const astra::Vector3f* srcRow = src + row * width;
		uint16_t* dstRow = _depthFrame->GetRowData<uint16_t>(row);

		for (int col = 0; col < width; col++)
		{
			const astra::Vector3f* pointData = srcRow + col;
			float depth = (*pointData).z;
			dstRow[col] = uint16_t(depth);
		}
	}
}

void AstraDevice::UpdateColor(astra::Frame& frame)
{
	assert(_colorFrame != nullptr);

	const astra::ColorFrame colorFrame = frame.get<astra::ColorFrame>();
	if (!colorFrame.is_valid())
	{
		// colorView_.texture_.update(&colorView_.buffer_[0]);
		return;
	}

	const int width = colorFrame.width();
	const int height = colorFrame.height();
	const astra::RgbPixel* src = colorFrame.data();
	assert(_colorFrame->GetElementSize() == sizeof(astra::RgbPixel));

	for (int row = 0; row < height; row++)
	{
		const astra::RgbPixel* srcRow = src + row * width;		
		void* dstRow = _colorFrame->GetRowData(row);

		memcpy(dstRow, srcRow, width * sizeof(astra::RgbPixel));
	}
}

void AstraDevice::UpdateInfrared(astra::Frame& frame)
{
	const astra::InfraredFrame16 infraredFrame = frame.get<astra::InfraredFrame16>();
	if (!infraredFrame.is_valid())
	{
		return;
	}

	const int width = infraredFrame.width();
	const int height = infraredFrame.height();
	const uint16_t* src = infraredFrame.data();
	assert(_infraredFrame->GetElementSize() == sizeof(uint16_t));

	for (int row = 0; row < height; row++)
	{
		const uint16_t* srcRow = src + row * width;
		void* dstRow = _infraredFrame->GetRowData(row);

		memcpy(dstRow, srcRow, width * sizeof(uint16_t));
	}
}

ComputeFramePtr AstraDevice::WaitReadFrame(EAstraStreamType streamType)
{
	astra_update();

	if (!_reader.is_valid())
	{
		return false;
	}

	// Get Last frame
	switch (streamType)
	{
	case frox::EAstraStreamType::STREAM_DEPTH:
		return _depthFrame;
	case frox::EAstraStreamType::STREAM_COLOR:
		return _colorFrame;
	case frox::EAstraStreamType::STREAM_INFRARED:
		return _infraredFrame;
	default:
		break;
	}

	return nullptr;
}

AstraSensorPtr AstraDevice::NewSensor(void* handle, const char* name)
{
	AstraSensorPtr sensor = AstraSensor::Create(handle);
	return sensor;
}

} // End frox.
