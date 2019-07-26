#include "ComputeFrameImplV1.h"
#include <assert.h>

namespace frox {

ComputeFrameImplV1::ComputeFrameImplV1(Size size, ComputeFrameType typeData, const void* data)
{
	assert(size.Width > 0 && size.Height > 0);
	assert(typeData.Type != EComputeFrameType::ECFT_None && typeData.Channels > 0);

	_data.Width = size.Width;
	_data.Height = size.Height;
	_data.Type = typeData.Type;
	_data.Channels = typeData.Channels;
	_data.Valid = true;

	if (_data.Width == 1 && _data.Height == 1 && _data.Channels == 1)
	{
		switch (_data.Type)
		{
		case ECFT_Bool:
			_data.Bool = data != nullptr ? *reinterpret_cast<const bool*>(data) : 0;
			break;
		case ECFT_UInt8:
			_data.Uint8 = data != nullptr ? *reinterpret_cast<const uint8_t*>(data) : 0;
			break;
		case ECFT_UInt16:
			_data.Uint16 = data != nullptr ? *reinterpret_cast<const uint16_t*>(data) : 0;
			break;
		case ECFT_UInt32:
			_data.Uint32 = data != nullptr ? *reinterpret_cast<const uint32_t*>(data) : 0;
			break;
		case ECFT_Float:
			_data.Scalar = data != nullptr ? *reinterpret_cast<const float*>(data) : 0.f;
			break;
		}
	}
	else
	{
		uint32_t nbElements = size.Width * size.Height;
		uint32_t elementSize = utils::FrameTypeToSize(typeData.Type);
		uint32_t nbBytes = nbElements * elementSize * _data.Channels;
		switch (typeData.Type)
		{
		case ECFT_Bool:
		case ECFT_UInt8:
		case ECFT_UInt16:
		case ECFT_UInt32:
		case ECFT_Float:
			_data.Data = malloc(nbBytes);
			if (data != nullptr)
			{
				memcpy(_data.Data, data, nbBytes);
			}
			else
			{
				memset(_data.Data, 0x00, nbBytes);
			}
			break;
		default:
			break;
		}
	}
}

ComputeFrameImplV1::~ComputeFrameImplV1()
{
	if (!_data.IsOptimized())
	{
		free(_data.Data);
	}
}

ComputeFrame::EClass ComputeFrameImplV1::GetClassType() const
{
	return Other;
}

ComputeFrameType ComputeFrameImplV1::GetType() const
{
	return ComputeFrameType{ _data.Type, _data.Channels };
}

uint32_t ComputeFrameImplV1::GetChannels() const
{
	return _data.Channels;
}

uint32_t ComputeFrameImplV1::GetElementSize() const
{
	return utils::FrameTypeToSize(_data.Type) * _data.Channels;
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
	if (_data.IsOptimized())
	{
		switch (_data.Type)
		{
		case ECFT_Bool:
			return &_data.Bool;
		case ECFT_UInt8:
			return &_data.Uint8;
		case ECFT_UInt16:
			return &_data.Uint16;
		case ECFT_UInt32:
			return &_data.Uint32;
		case ECFT_Float:
			return &_data.Scalar;
		}
	}

	return _data.Data;
}

const void* ComputeFrameImplV1::GetRowData(uint32_t row) const
{
	assert(row < _data.Height);
	return _data.IsOptimized() ?
		GetData() :
		reinterpret_cast<uint8_t*>(_data.Data) + (row * _data.Width) * GetElementSize();
}

const void* ComputeFrameImplV1::At(uint32_t row, uint32_t column) const
{
	assert(row < _data.Height && column < _data.Width);
	return _data.IsOptimized() ?
		GetData() :
		reinterpret_cast<uint8_t*>(_data.Data) + (row * _data.Width + column) * GetElementSize();
}

void* ComputeFrameImplV1::GetData()
{
	if (_data.IsOptimized())
	{
		switch (_data.Type)
		{
		case ECFT_Bool:
			return &_data.Bool;
		case ECFT_UInt8:
			return &_data.Uint8;
		case ECFT_UInt16:
			return &_data.Uint16;
		case ECFT_UInt32:
			return &_data.Uint32;
		case ECFT_Float:
			return &_data.Scalar;
		}
	}

	return _data.Data;
}

void* ComputeFrameImplV1::GetRowData(uint32_t row)
{
	assert(row < _data.Height);
	return _data.IsOptimized() ?
		GetData() :
		reinterpret_cast<uint8_t*>(_data.Data) + (row * _data.Width) * GetElementSize();
}

void* ComputeFrameImplV1::At(uint32_t row, uint32_t column)
{
	assert(row < _data.Height && column < _data.Width);
	return _data.IsOptimized() ?
		GetData() :
		reinterpret_cast<uint8_t*>(_data.Data) + (row * _data.Width + column) * GetElementSize();
}

} // End frox