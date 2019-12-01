#include "Kinect2Sensor.h"
#include "Kinect2Device.h"
#include "Kinect2Module.h"

#include <algorithm>

namespace frox {

Kinect2Sensor::Kinect2Sensor(void* hSensor)
	: _hSensor(hSensor)
{}

Kinect2Sensor::~Kinect2Sensor()
{
	// close the Sensor
}

void Kinect2Sensor::QueryData()
{
	// Read Info nad profilers
}

bool Kinect2Sensor::FindProfile(Kinect2StreamProfile profile, Kinect2StreamProfile& out) const
{
	auto it = std::find_if(_streamProfiles.begin(), _streamProfiles.end(), [profile](const Kinect2StreamProfile& other) {
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

bool Kinect2Sensor::SupportsProfile(Kinect2StreamProfile profile) const
{
	auto it = std::find_if(_streamProfiles.begin(), _streamProfiles.end(), [profile](const Kinect2StreamProfile& other) {
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
