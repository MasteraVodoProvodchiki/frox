#include "AstraSensor.h"
#include "AstraDevice.h"
#include "AstraModule.h"


namespace frox {

AstraSensor::AstraSensor(void* hSensor)
	: _hSensor(hSensor)
{}

AstraSensor::~AstraSensor()
{
	// close the Sensor
}

void AstraSensor::QueryData()
{
	// Read Info nad profilers
}

bool AstraSensor::FindProfile(AstraStreamProfile profile, AstraStreamProfile& out) const
{
	auto it = std::find_if(_streamProfiles.begin(), _streamProfiles.end(), [profile](const AstraStreamProfile& other) {
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

bool AstraSensor::SupportsProfile(AstraStreamProfile profile) const
{
	auto it = std::find_if(_streamProfiles.begin(), _streamProfiles.end(), [profile](const AstraStreamProfile& other) {
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
