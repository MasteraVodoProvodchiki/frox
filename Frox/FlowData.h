#pragma once

#include "Common.h"
#include "ComputeFrame.h"

namespace frox {

class FROX_API FlowData
{
public:
	virtual ~FlowData() {}

	virtual void SetFrame(const char* name, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const char* name) const = 0;

	void SetInputScalar(const char* name, uint8_t value);
	void SetInputScalar(const char* name, uint16_t value);
	void SetInputScalar(const char* name, uint32_t value);
	void SetInputScalar(const char* name, float value);

	void Release();
};

} // End frox