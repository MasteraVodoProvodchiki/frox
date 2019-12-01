#pragma once

#include "Native.h"
#include "Types.h"

namespace frox {

enum class EAstraSensorType : uint8_t
{
	Depth = 0,
	Color,
	Infrared
};

using Kinect2SensorPtr = std::shared_ptr<class Kinect2Sensor>;
class Kinect2Sensor : std::enable_shared_from_this<Kinect2Sensor>
{
public:
	Kinect2Sensor(void* hSensor);
	virtual ~Kinect2Sensor();

	static Kinect2SensorPtr Create(void* hSensor)
	{
		return std::make_shared<Kinect2Sensor>(hSensor);
	}

	void QueryData();
	bool FindProfile(Kinect2StreamProfile profile, Kinect2StreamProfile& out) const;
	bool SupportsProfile(Kinect2StreamProfile profile) const;

	const std::string& GetName() const { return _name; }

protected:
	std::string _name;
	void* _hSensor;

private:
	std::vector<Kinect2StreamProfile> _streamProfiles;
};

} // End frox.
