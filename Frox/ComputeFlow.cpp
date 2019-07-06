#include "ComputeFlow.h"
#include "ComputeNode.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

bool ComputeFlow::ConnectEntry(const char* entryName, ComputeNode* inNode, const char* inPinName)
{
	// Find Entry
	int32_t entryIndex = FindEntryByName(entryName);
	if (entryIndex == -1)
	{
		return false;
	}

	// Find Pin
	int32_t inPinIndex = inNode->FindInputByName(inPinName);
	if (inPinIndex == -1)
	{
		return false;
	}

	// Connect
	ConnectEntry(entryIndex, inNode, inPinIndex);
	return true;
}

bool ComputeFlow::DisconnectEntry(const char* entryName, ComputeNode* inNode, const char* inPinName)
{
	// Find Entry
	int32_t entryIndex = FindEntryByName(entryName);
	if (entryIndex == -1)
	{
		return false;
	}

	// Find Pin
	int32_t inPinIndex = inNode->FindInputByName(inPinName);
	if (inPinIndex == -1)
	{
		return false;
	}

	// Disconnect
	DisconnectEntry(entryIndex, inNode, inPinIndex);
	return true;
}

bool ComputeFlow::ConnectOutput(const char* outputName, ComputeNode* outNode, const char* outPinName)
{
	// Find Entry
	int32_t outputIndex = FindOutputByName(outputName);
	if (outputIndex == -1)
	{
		return false;
	}

	// Find Pin
	int32_t outPinIndex = outNode->FindOutputByName(outPinName);
	if (outPinIndex == -1)
	{
		return false;
	}

	// Connect
	ConnectEntry(outputIndex, outNode, outPinIndex);
	return true;
}

bool ComputeFlow::DisconnectOutput(const char* outputName, ComputeNode* outNode, const char* outPinName)
{
	// Find Entry
	int32_t outputIndex = FindOutputByName(outputName);
	if (outputIndex == -1)
	{
		return false;
	}

	// Find Pin
	int32_t outPinIndex = outNode->FindOutputByName(outPinName);
	if (outPinIndex == -1)
	{
		return false;
	}

	// Connect
	DisconnectOutput(outputIndex, outNode, outPinIndex);
	return true;
}

uint32_t ComputeFlow::FindOrCreateEntry(const char* name, EPinValueType type)
{
	assert(name != nullptr);

	uint32_t entryId = this->FindEntryByName(name);
	return entryId != -1 ? entryId : this->CreateEntry(name, type);
}

uint32_t ComputeFlow::FindOrCreateOutput(const char* name, EPinValueType type)
{
	assert(name != nullptr);

	uint32_t outputId = this->FindOutputByName(name);
	return outputId != -1 ? outputId : this->CreateOutput(name, type);
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