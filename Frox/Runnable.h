#pragma once

namespace frox {

class Runnable
{
public:
	virtual ~Runnable() {}
	virtual void Run() = 0;
};

} // End frox
