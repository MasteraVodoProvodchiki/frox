#pragma once

#include "Common.h"

namespace frox {

class FROX_API FlowData
{
public:
	virtual ~FlowData() {}
	virtual void SetFrame(const char* name, ComputeFrame* frame) = 0;

	void Release();
};

} // End frox