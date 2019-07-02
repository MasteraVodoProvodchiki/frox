#include "FlowData.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

void FlowData::SetInputScalar(const char* name, uint8_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt8, &value);
	this->SetFrame(name, frame);
}

void FlowData::SetInputScalar(const char* name, uint16_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt16, &value);
	this->SetFrame(name, frame);
}

void FlowData::SetInputScalar(const char* name, uint32_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt32, &value);
	this->SetFrame(name, frame);
}

void FlowData::SetInputScalar(const char* name, float value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_Float, &value);
	this->SetFrame(name, frame);
}

void FlowData::Release()
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	frox->DestroyFlowData(this);
}

} // End frox