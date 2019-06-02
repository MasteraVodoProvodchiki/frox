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
			case frox::ECFT_UInt8:
				_data.Uint8 = data != nullptr ? *reinterpret_cast<uint8_t*>(data) : 0;
				break;
			case frox::ECFT_Float:
				_data.Scalar = data != nullptr ? *reinterpret_cast<float*>(data) : 0.f;
				break;
			}
		}
		else
		{
			switch (type)
			{
			case frox::ECFT_UInt8:
				_data.Data = malloc(size.Width * size.Height * sizeof(uint8_t));
				if (data != nullptr)
				{
					memcpy(_data.Data, data, size.Width * size.Height * sizeof(uint8_t));
				}
				break;
			case frox::ECFT_Float:
				_data.Data = malloc(size.Width * size.Height * sizeof(float));
				if (data != nullptr)
				{
					memcpy(_data.Data, data, size.Width * size.Height * sizeof(float));
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

void* ComputeFrameImplV1::GetData() const
{
	return _data.Data;
}

} // End frox