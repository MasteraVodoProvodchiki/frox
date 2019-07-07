#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "Variant.h"

namespace frox {

class FROX_API FlowData
{
public:
	virtual ~FlowData() {}

	virtual void SetValue(const char* name, Variant value) = 0;
	virtual Variant GetValue(const char* name) const = 0;
	virtual void SetFrame(const char* name, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const char* name) const = 0;

	template<typename T>
	T GetValue(const char* name) const
	{
		Variant value = this->GetValue(name);
		return value.To<T>();
	}

	// Set scalars as frame
	void SetInputScalar(const char* name, uint8_t value);
	void SetInputScalar(const char* name, uint16_t value);
	void SetInputScalar(const char* name, uint32_t value);
	void SetInputScalar(const char* name, float value);

	void Release();
};

} // End frox