#include "ComputeFlow.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

uint32_t ComputeFlow::FindOrCreateEntry(const char* name)
{
	assert(name != nullptr);

	uint32_t entryId = this->FindEntryByName(name);
	return entryId != -1 ? entryId : this->CreateEntry(name);
}

uint32_t ComputeFlow::FindOrCreateOutput(const char* name)
{
	assert(name != nullptr);

	uint32_t outputId = this->FindOutputByName(name);
	return outputId != -1 ? outputId : this->CreateOutput(name);
}

/*
void ComputeFlow::SetInputScalar(uint32_t inId, uint8_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt8, &value);
	this->SetInput(inId, frame);
}

void ComputeFlow::SetInputScalar(uint32_t inId, uint16_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt16, &value);
	this->SetInput(inId, frame);
}

void ComputeFlow::SetInputScalar(uint32_t inId, uint32_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt32, &value);
	this->SetInput(inId, frame);
}

void ComputeFlow::SetInputScalar(uint32_t inId, float value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_Float, &value);
	this->SetInput(inId, frame);
}
*/

void ComputeFlow::Release()
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	frox->DestroyComputeFlow(this);
}

} // End frox