#include "ComputeNode.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

/*
void ComputeNode::SetInputScalar(uint32_t inId, uint8_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt8, &value);
	this->SetInput(inId, frame);
}

void ComputeNode::SetInputScalar(uint32_t inId, uint16_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt16, &value);
	this->SetInput(inId, frame);
}

void ComputeNode::SetInputScalar(uint32_t inId, uint32_t value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_UInt32, &value);
	this->SetInput(inId, frame);
}

void ComputeNode::SetInputScalar(uint32_t inId, float value)
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	ComputeFramePtr frame = frox->CreateComputeFrame(Size{ 1, 1 }, ECFT_Float, &value);
	this->SetInput(inId, frame);
}
*/

} // End frox