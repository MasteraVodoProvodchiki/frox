#include "ComputeFrameImplV1.h"
#include <assert.h>

namespace frox {

ComputeFrameImplV1::ComputeFrameImplV1(Size size, EComputeFrameType type, void* data)
{
	if (size.Width > 0 && size.Height > 0 && type != ECFT_None)
	{
		_data.Width = size.Width;
		_data.Height = size.Height;
		_data.Type = type;

		_data.Valid = true;

		if (size.Width == 1 && size.Height == 1)
		{
			switch (type)
			{
			case ECFT_UInt8:
				_data.Uint8 = data != nullptr ? *reinterpret_cast<uint8_t*>(data) : 0;
				break;
			case ECFT_Float:
				_data.Scalar = data != nullptr ? *reinterpret_cast<float*>(data) : 0.f;
				break;
			}
		}
		else
		{
			uint32_t nbElements = size.Width * size.Height;
			uint32_t elementSize = utils::FrameTypeToSize(type);
			switch (type)
			{
			case ECFT_UInt8:
				_data.Data = malloc(nbElements * elementSize);
				if (data != nullptr)
				{
					memcpy(_data.Data, data, nbElements * elementSize);
				}
				break;
			case ECFT_Float:
				_data.Data = malloc(nbElements * elementSize);
				if (data != nullptr)
				{
					memcpy(_data.Data, data, nbElements * elementSize);
				}
				break;
			}
		}
	}
}

ComputeFrameImplV1::~ComputeFrameImplV1()
{
	if (_data.Width > 1 && _data.Height > 1)
	{
		free(_data.Data);
	}
}

EComputeFrameType ComputeFrameImplV1::GetType() const
{
	return _data.Type;
}

uint32_t ComputeFrameImplV1::GetElementSize() const
{
	return utils::FrameTypeToSize(_data.Type);
}

bool ComputeFrameImplV1::IsValid() const
{
	return _data.Valid;
}

Size ComputeFrameImplV1::GetSize() const
{
	return Size{
		_data.Width,
		_data.Height
	};
}

const void* ComputeFrameImplV1::GetData() const
{
	return (_data.Width == 1 && _data.Height == 1) ? &_data.Mem : _data.Data;
}

const void* ComputeFrameImplV1::GetRowData(uint32_t row) const
{
	return reinterpret_cast<uint8_t*>(_data.Data) + row * utils::FrameTypeToSize(_data.Type);
}

void* ComputeFrameImplV1::GetData()
{
	return (_data.Width == 1 && _data.Height == 1) ? &_data.Mem : _data.Data;
}

void* ComputeFrameImplV1::GetRowData(uint32_t row)
{
	return reinterpret_cast<uint8_t*>(_data.Data) + row * utils::FrameTypeToSize(_data.Type);
}

} // End frox