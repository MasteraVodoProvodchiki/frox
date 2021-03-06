#include "ComputeFrame.h"
#include <assert.h>

namespace frox {
namespace utils {

/* I0, O0 */
template<typename LeftT, typename RightT,typename CallbackT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, CallbackT callback)
{
	Size leftSize = left->GetSize();
	Size rightSize = right->GetSize();

	assert(leftSize == rightSize);

	for (uint32_t row = 0; row < leftSize.Height; ++row)
	{
		const LeftT* leftValues = left->GetRowData<LeftT>(row);
		RightT* rightValues = right->GetRowData<RightT>(row);

		for (uint32_t column = 0; column < leftSize.Width; ++column)
		{
			callback(leftValues[column], rightValues[column]);
		}
	}
}

/* I0, O0 */
template <typename LeftT, typename OperatorT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, OperatorT callback)
{
	ComputeFrameType rightType = right->GetType();
	switch (rightType.Type)
	{
	case ECFT_Bool: {
		Foreach<LeftT, bool>(left, right, callback);
		break;
	}
	case ECFT_UInt8: {
		Foreach<LeftT, uint8_t>(left, right, callback);
		break;
	}
	case ECFT_UInt16: {
		Foreach<LeftT, uint16_t>(left, right, callback);
		break;
	}
	case ECFT_UInt32: {
		Foreach<LeftT, uint32_t>(left, right, callback);
		break;
	}
	case ECFT_Float: {
		Foreach<LeftT, float>(left, right, callback);
		break;
	}
	default:
		assert(false);
		break;
	}
}

/* I0, O0 */
template <typename OperatorT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, OperatorT callback)
{
	ComputeFrameType leftType = left->GetType();

	switch (leftType.Type)
	{
	case ECFT_Bool: {
		Foreach<bool>(left, right, callback);
		break;
	}
	case ECFT_UInt8: {
		Foreach<uint8_t>(left, right, callback);
		break;
	}
	case ECFT_UInt16: {
		Foreach<uint16_t>(left, right, callback);
		break;
	}
	case ECFT_UInt32: {
		Foreach<uint32_t>(left, right, callback);
		break;
	}
	case ECFT_Float: {
		Foreach<float>(left, right, callback);
		break;
	}
	default:
		assert(false);
		break;
	}
}

/* I0, I1, O0 */
template<typename TypeT, typename CallbackT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, ComputeFramePtr output, CallbackT callback)
{
	Size leftSize = left->GetSize();
	Size rightSize = right->GetSize();
	Size outputSize = output->GetSize();
	Size size = outputSize;

	assert(leftSize == outputSize);
	assert(rightSize == outputSize);

	for (uint32_t row = 0; row < size.Height; ++row)
	{
		const TypeT* leftValues = left->GetRowData<TypeT>(row);
		const TypeT* rightValues = right->GetRowData<TypeT>(row);
		TypeT* outputValues = output->GetRowData<TypeT>(row);
		
		for (uint32_t column = 0; column < size.Width; ++column)
		{
			outputValues[column] = callback(leftValues[column], rightValues[column]);
		}
	}
}

/* I0, I1, O0 */
template <typename OperatorT>
void Foreach(ComputeFramePtr left, ComputeFramePtr right, ComputeFramePtr output, OperatorT callback)
{
	ComputeFrameType leftType = left->GetType();
	ComputeFrameType rightType = right->GetType();
	ComputeFrameType outputType = output->GetType();
	ComputeFrameType type = outputType;

	assert(leftType == outputType);
	assert(rightType == outputType);

	switch (type.Type)
	{
	case ECFT_Bool: {
		Foreach<bool>(left, right, output, callback);
		break;
	}
	case ECFT_UInt8: {
		Foreach<uint8_t>(left, right, output, callback);
		break;
	}
	case ECFT_UInt16: {
		Foreach<uint16_t>(left, right, output, callback);
		break;
	}
	case ECFT_UInt32: {
		Foreach<uint32_t>(left, right, output, callback);
		break;
	}
	case ECFT_Float: {
		Foreach<float>(left, right, output, callback);
		break;
	}
	default:
		assert(false);
		break;
	}
}

template <typename TypeT, typename CallbackT>
void Foreach(ComputeFramePtr frame, CallbackT callback)
{
	Size size = frame->GetSize();

	for (uint32_t row = 0; row < size.Height; ++row)
	{
		const TypeT* values = frame->GetRowData<TypeT>(row);
		for (uint32_t column = 0; column < size.Width; ++column)
		{
			callback(values[column]);
		}
	}
}

template <typename TypeT, typename CallbackT>
void Fill(ComputeFramePtr frame, CallbackT callback)
{
	Size size = frame->GetSize();
	uint32_t nbChannels = frame->GetChannels();
	for (uint32_t row = 0; row < size.Height; ++row)
	{
		TypeT* values = frame->GetRowData<TypeT>(row);
		for (uint32_t column = 0; column < size.Width; ++column)
		{
			uint32_t index = column * nbChannels;
			for (uint32_t channel = 0; channel < nbChannels; ++channel)
			{	
				values[index + channel] = callback(values[column + channel]);
			}
		}
	}
}

FROX_API void Copy(ComputeFramePtr src, ComputeFramePtr dst);

} // End utils
} // End frox