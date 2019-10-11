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

ComputeFramePtr makeCircle(Size size, uint32_t radius)
{
	auto gFrox = FroxInstance();
	assert(gFrox != nullptr);

	// TODO. std::vector<bool> doesn't contain method data
	uint8_t* values = new uint8_t[size.Width * size.Height];
	float radiusf = float(radius);
	for (uint32_t x = 0; x < size.Width; ++x)
	{
		for (uint32_t y = 0; y < size.Height; ++y)
		{
			float d = std::sqrtf(
				std::powf(int(x) - int(size.Width / 2), 2.f) +
				std::powf(int(y) - int(size.Height / 2), 2.f)
			);
			values[y * size.Width + x] = d < radiusf ? 255 : 0;
		}
	}


	auto frame = gFrox->CreateComputeFrame(size, EComputeFrameType::ECFT_UInt8, values);

	delete[] values;

	return frame;
}

frox::ComputeFramePtr makeBoxByType(frox::Size size, frox::Point offset, frox::Size extent, frox::EComputeFrameType type, Variant in, Variant out)
{
	switch (type)
	{
	case ECFT_Bool:
		return makeBox<bool>(size, offset, extent, type, in.To<bool>(), out.To<bool>());
	case ECFT_UInt8:
		return makeBox<uint8_t>(size, offset, extent, type, in.To<uint8_t>(), out.To<uint8_t>());
	case ECFT_UInt16:
		return makeBox<uint16_t>(size, offset, extent, type, in.To<uint16_t>(), out.To<uint16_t>());
	case ECFT_UInt32:
		return makeBox<uint32_t>(size, offset, extent, type, in.To<uint32_t>(), out.To<uint32_t>());
	case ECFT_Float:
		return makeBox<float>(size, offset, extent, type, in.To<float>(), out.To<float>());
	default:
		return nullptr;
	}

	return nullptr;
}