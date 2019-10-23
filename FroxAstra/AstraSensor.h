#pragma once

#include "AstraNative.h"
#include "AstraTypes.h"

namespace frox {

using AstraSensorPtr = std::shared_ptr<class AstraSensor>;
class AstraSensor : std::enable_shared_from_this<AstraSensor>
{
public:
	AstraSensor(void* hSensor);
	virtual ~AstraSensor();

	static AstraSensorPtr Create(void* hSensor)
	{
		return std::make_shared<AstraSensor>(hSensor);
	}

	void QueryData();
	bool FindProfile(AstraStreamProfile profile, AstraStreamProfile& out) const;
	bool SupportsProfile(AstraStreamProfile profile) const;

	const std::string& GetName() const { return _name; }

protected:
	std::string _name;
	void* _hSensor;

private:
	std::vector<AstraStreamProfile> _streamProfiles;
};

} // End frox.
