#include "RealSenseSensor.h"
#include "RealSenseDevice.h"
#include "RealSenseModule.h"


namespace frox {

RealSenseSensor::RealSenseSensor(rs2_sensor* rsSensor)
	: _rsSensor(rsSensor)
{}

RealSenseSensor::~RealSenseSensor()
{
	// close the Kinect Sensor
	if (_rsSensor != nullptr)
	{
		rs2_delete_sensor(_rsSensor);
	}
}

void RealSenseSensor::QueryData()
{
	rs2::error_ref e;
	_name = rs2_get_sensor_info(_rsSensor, RS2_CAMERA_INFO_NAME, &e);

	/*
	for (int i = 0; i < RS2_OPTION_COUNT; ++i)
	{
		if (SupportsOption((ERealSenseOptionType)i))
		{
			Options.Add(NewOption((rs2_options*)_rsSensor, (ERealSenseOptionType)i, TEXT("RealSenseOption")));
		}
	}
	*/

	rs2_stream_profile_list* profileList = rs2_get_stream_profiles(_rsSensor, &e);
	if (profileList != nullptr)
	{
		const int numProfiles = rs2_get_stream_profiles_count(profileList, &e);
		for (int id = 0; id < numProfiles; id++)
		{
			const rs2_stream_profile* profile = rs2_get_stream_profile(profileList, id, &e);
			if (profile != nullptr)
			{
				rs2_stream stream;
				rs2_format format;
				int streamId;
				int profileId;
				int framerate;
				rs2_get_stream_profile_data(profile, &stream, &format, &streamId, &profileId, &framerate, &e);

				if (e.success())
				{
					if (
						(stream == RS2_STREAM_DEPTH && format == RS2_FORMAT_Z16)
						|| (stream == RS2_STREAM_COLOR && format == RS2_FORMAT_RGBA8)
						|| (stream == RS2_STREAM_INFRARED && format == RS2_FORMAT_Y8)
					)
					{
						RealSenseStreamProfile spi;
						spi.StreamType = (ERealSenseStreamType)stream;
						spi.Format = (ERealSenseFormatType)format;
						spi.Rate = framerate;
						rs2_get_video_stream_resolution(profile, &spi.Width, &spi.Height, &e);
						rs2_get_video_stream_intrinsics(profile, &spi.Intrinsics, &e);

						if (e.success())
						{
							_streamProfiles.push_back(spi);
						}
					}
				}
			}
		} // End for each profile
	} // End profileList
}

bool RealSenseSensor::FindProfile(RealSenseStreamProfile profile, RealSenseStreamProfile& out) const
{
	auto it = std::find_if(_streamProfiles.begin(), _streamProfiles.end(), [profile](const RealSenseStreamProfile& other) {
		return
			other.StreamType == profile.StreamType
			&& other.Format == profile.Format
			&& other.Width == profile.Width
			&& other.Height == profile.Height
			&& other.Rate == profile.Rate;
	});

	if (it != _streamProfiles.end())
	{
		out = *it;
		return true;
	}

	return false;
}

bool RealSenseSensor::SupportsProfile(RealSenseStreamProfile profile) const
{
	auto it = std::find_if(_streamProfiles.begin(), _streamProfiles.end(), [profile](const RealSenseStreamProfile& other) {
		return
			other.StreamType == profile.StreamType
			&& other.Format == profile.Format
			&& other.Width == profile.Width
			&& other.Height == profile.Height
			&& other.Rate == profile.Rate;
	});
	return it != _streamProfiles.end();
}

} // End frox.
