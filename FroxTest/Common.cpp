#include "Common.h"

using namespace frox;

bool checkRangeAuto(ComputeFramePtr frame)
{
	EComputeFrameType type = frame->GetType().Type;
	switch (type)
	{
	case ECFT_Bool:
		return checkRange<bool>(frame, false, true);
	case ECFT_UInt8:
		return checkRange<uint8_t>(frame, 0, std::numeric_limits<uint8_t>::max());
	case ECFT_UInt16:
		return checkRange<uint16_t>(frame, 0, std::numeric_limits<uint16_t>::max());
	case ECFT_UInt32:
		return checkRange<uint32_t>(frame, 0, std::numeric_limits<uint32_t>::max());
	case ECFT_Float:
		return checkRange<float>(frame, 0.f, 1.f);
	default:
		return false;
	}

	return false;
}

bool checkSumOne(ComputeFramePtr frame, uint32_t nbElements)
{
	EComputeFrameType type = frame->GetType().Type;
	switch (type)
	{
	case ECFT_Bool:
		return checkSum<bool, uint32_t>(frame, nbElements);
	case ECFT_UInt8:
		return checkSum<uint8_t, uint32_t>(frame, nbElements);
	case ECFT_UInt16:
		return checkSum<uint16_t, uint32_t>(frame, nbElements);
	case ECFT_UInt32:
		return checkSum<uint32_t, uint32_t>(frame, nbElements);
	case ECFT_Float:
		return checkSum<float, float>(frame, float(nbElements));
	default:
		return false;
	}

	return false;
}

template<typename T>
ComputeFramePtr makeFrame(Size size, EComputeFrameType type, T value)
{
	auto gFrox = FroxInstance();
	assert(gFrox != nullptr);

	// TODO. std::vector<bool> doesn't contain method data
	T* values = new T[size.Width * size.Height];
	for (uint32_t i = 0; i < size.Width * size.Height; ++i)
	{
		values[i] = value;
	}

	auto frame = gFrox->CreateComputeFrame(size, type, values);

	delete[] values;

	return frame;
}

ComputeFramePtr makeFrame(Size size, EComputeFrameType type, Variant value)
{
	switch (type)
	{
	case ECFT_Bool:
		return makeFrame<bool>(size, type, value.To<bool>());
	case ECFT_UInt8:
		return makeFrame<uint8_t>(size, type, value.To<uint8_t>());
	case ECFT_UInt16:
		return makeFrame<uint16_t>(size, type, value.To<uint16_t>());
	case ECFT_UInt32:
		return makeFrame<uint32_t>(size, type, value.To<uint32_t>());
	case ECFT_Float:
		return makeFrame<float>(size, type, value.To<float>());
	default:
		return nullptr;
	}

	return nullptr;
}