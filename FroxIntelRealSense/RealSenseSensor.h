#pragma once

#include "RealSenseNative.h"
#include "RealSenseTypes.h"

namespace frox {

using RealSenseSensorPtr = std::shared_ptr<class RealSenseSensor>;
class RealSenseSensor : std::enable_shared_from_this<RealSenseSensor>
{
public:
	RealSenseSensor(struct rs2_sensor* rsSensor);
	virtual ~RealSenseSensor();

	static RealSenseSensorPtr Create(struct rs2_sensor* rsSensor)
	{
		return std::make_shared<RealSenseSensor>(rsSensor);
	}

	void QueryData();
	bool FindProfile(RealSenseStreamProfile profile, RealSenseStreamProfile& out) const;
	bool SupportsProfile(RealSenseStreamProfile profile) const;

	const std::string& GetName() const { return _name; }

protected:
	std::string _name;
	struct rs2_sensor* _rsSensor;

private:
	std::vector<RealSenseStreamProfile> _streamProfiles;
};

} // End frox.
