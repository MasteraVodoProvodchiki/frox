#pragma once

#include "Common.h"
#include "Types.h"

#include <assert.h>
#include <inttypes.h>

namespace frox {

class Variant
{
	enum EType
	{
		ET_None,
		ET_Bool,
		ET_Int,
		// ET_Int2,
		// ET_Int3,
		// ET_Int4,
		ET_Uint,
		// ET_Uint2,
		// ET_Uint3,
		// ET_Uint4,
		ET_Float,
		ET_Float2,
		ET_Float3,
		ET_Float4,
		ET_Point,
		ET_Size
	};

public:
	inline Variant()
		: _type(ET_None)
	{}

	inline Variant(bool value)
		: _type(ET_Bool)
		, _bool(value)
	{}

	inline Variant(int value)
		: _type(ET_Int)
		, _int(value)
	{}

	inline Variant(uint32_t value)
		: _type(ET_Uint)
		, _int(value)
	{}

	inline Variant(float value)
		: _type(ET_Float)
		, _float(value)
	{}

	inline Variant(float2 value)
		: _type(ET_Float2)
		, _float2(value)
	{}

	inline Variant(float3 value)
		: _type(ET_Float3)
		, _float3(value)
	{}

	inline Variant(float4 value)
		: _type(ET_Float4)
		, _float4(value)
	{}

	inline Variant(Point value)
		: _type(ET_Point)
		, _point(value)
	{}

	inline Variant(Size value)
		: _type(ET_Point)
		, _size(value)
	{}

	inline bool IsValid() const
	{
		return _type != ET_None;
	}

	template<typename T>
	inline bool Is() const
	{
		return false;
	}

	template<typename T>
	inline T As() const
	{
		return T();
	}

	template<typename T>
	inline T To() const
	{
		return T();
	}

private:
	EType _type;
	union
	{
		bool _bool;
		int _int;
		uint32_t _uint32;
		float _float;
		float2 _float2;
		float3 _float3;
		float4 _float4;
		Point _point;
		Size _size;
	};
};

// Is
template<>
inline bool Variant::Is<bool>() const
{
	return _type == ET_Bool;
}

template<>
inline bool Variant::Is<uint8_t>() const
{
	return _type == ET_Int;
}

template<>
inline bool Variant::Is<uint16_t>() const
{
	return _type == ET_Int;
}

template<>
inline bool Variant::Is<int>() const
{
	return _type == ET_Int;
}

template<>
inline bool Variant::Is<uint32_t>() const
{
	return _type == ET_Uint;
}

template<>
inline bool Variant::Is<float>() const
{
	return _type == ET_Float;
}

template<>
inline bool Variant::Is<float2>() const
{
	return _type == ET_Float2;
}

template<>
inline bool Variant::Is<float3>() const
{
	return _type == ET_Float3;
}

template<>
inline bool Variant::Is<float4>() const
{
	return _type == ET_Float4;
}

template<>
inline bool Variant::Is<Point>() const
{
	return _type == ET_Point;
}

template<>
inline bool Variant::Is<Size>() const
{
	return _type == ET_Size;
}

// As
template<>
inline bool Variant::As<bool>() const
{
	assert(_type == ET_Bool);
	return _bool;
}

template<>
inline uint8_t Variant::As<uint8_t>() const
{
	assert(_type == ET_Int);
	return uint8_t(_int);
}

template<>
inline uint16_t Variant::As<uint16_t>() const
{
	assert(_type == ET_Int);
	return uint16_t(_int);
}

template<>
inline int Variant::As<int>() const
{
	assert(_type == ET_Int);
	return _int;
}

template<>
inline uint32_t Variant::As<uint32_t>() const
{
	assert(_type == ET_Uint);
	return _uint32;
}

template<>
inline float Variant::As<float>() const
{
	assert(_type == ET_Float);
	return _float;
}

template<>
inline float2 Variant::As<float2>() const
{
	assert(_type == ET_Float2);
	return _float2;
}

template<>
inline float3 Variant::As<float3>() const
{
	assert(_type == ET_Float3);
	return _float3;
}

template<>
inline float4 Variant::As<float4>() const
{
	assert(_type == ET_Float4);
	return _float4;
}

template<>
inline Point Variant::As<Point>() const
{
	assert(_type == ET_Point);
	return _point;
}

template<>
inline Size Variant::As<Size>() const
{
	assert(_type == ET_Size);
	return _size;
}

// To
template<>
inline bool Variant::To<bool>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool;
	case ET_Int:
		return _int > 0;
	case ET_Uint:
		return _uint32 > 0;
	case ET_Float:
		return _float > 0;
	default:
		return false;
	}
}

template<>
inline uint8_t Variant::To<uint8_t>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return uint8_t(_int);
	case ET_Uint:
		return uint8_t(_int);
	case ET_Float:
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return uint8_t(_float);
	default:
		return 0;
	}
}

template<>
inline uint16_t Variant::To<uint16_t>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return uint16_t(_int);
	case ET_Uint:
		return uint16_t(_int);
	case ET_Float:
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return uint16_t(_float);
	default:
		return 0;
	}
}

template<>
inline int Variant::To<int>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return _int;
	case ET_Uint:
		return int(_uint32);
	case ET_Float:
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return int(_float);
	default:
		return 0;
	}
}

template<>
inline uint32_t Variant::To<uint32_t>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return uint32_t(_int);
	case ET_Uint:
		return _uint32;
	case ET_Float:
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return uint32_t(_float);
	default:
		return 0;
	}
}

template<>
inline float Variant::To<float>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1.f : 0.f;
	case ET_Int:
		return float(_int);
	case ET_Uint:
		return float(_uint32);
	case ET_Float:
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return _float;
	default:
		return 0.f;
	}
}

template<>
inline float2 Variant::To<float2>() const
{
	switch (_type)
	{
	case ET_Int:
		return float2{ float(_int), 0.f };
	case ET_Uint:
		return float2{ float(_uint32), 0.f };
	case ET_Float:
		return float2{ _float, 0.f };
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return _float2;
	default:
		return float2{ 0.f, 0.f };
	}
}

template<>
inline float3 Variant::To<float3>() const
{
	switch (_type)
	{
	case ET_Int:
		return float3{ float(_int), 0.f, 0.f };
	case ET_Uint:
		return float3{ float(_uint32), 0.f, 0.f };
	case ET_Float:
		return float3{ _float, 0.f, 0.f };
	case ET_Float2:
		return float3{ _float2.X, _float2.Y, 0.f };
	case ET_Float3:
	case ET_Float4:
		return _float3;
	default:
		return float3{ 0.f, 0.f, 0.f };
	}
}

template<>
inline float4 Variant::To<float4>() const
{
	switch (_type)
	{
	case ET_Int:
		return float4{ float(_int), 0.f, 0.f, 0.f };
	case ET_Uint:
		return float4{ float(_uint32), 0.f, 0.f, 0.f };
	case ET_Float:
		return float4{ _float, 0.f, 0.f, 0.f };
	case ET_Float2:
		return float4{ _float2.X, _float2.Y, 0.f, 0.f };
	case ET_Float3:
		return float4{ _float3.X, _float3.Y, _float3.Z, 0.f };
	case ET_Float4:
		return _float4;
	default:
		return float4{ 0.f, 0.f, 0.f, 0.f };
	}
}

template<>
inline Point Variant::To<Point>() const
{
	switch (_type)
	{
	case ET_Bool:
		return Point({ int32_t(_bool), int32_t(_bool) });
	case ET_Int:
		return Point({ _int, _int });
	case ET_Uint:
		return Point({ int32_t(_uint32), int32_t(_uint32) });
	case ET_Float:
		return Point({ int32_t(_float), int32_t(_float) });
	case ET_Float2:	
	case ET_Float3:
	case ET_Float4:
		return Point{ int32_t(_float2.X), int32_t(_float2.Y) };
	case ET_Point:
		return _point;
	case ET_Size:
		return Point({ int32_t(_size.Width), int32_t(_size.Height) });
	default:
		return Point({ 0, 0 });
	}
}

template<>
inline Size Variant::To<Size>() const
{
	switch (_type)
	{
	case ET_Bool:
		return Size({ uint32_t(_bool), uint32_t(_bool) });
	case ET_Int:
		return Size({ uint32_t(_int), uint32_t(_int) });
	case ET_Uint:
		return Size({ _uint32, _uint32 });
	case ET_Float:
		return Size({ uint32_t(_float), uint32_t(_float) });
	case ET_Float2:
	case ET_Float3:
	case ET_Float4:
		return Size{ uint32_t(_float2.X), uint32_t(_float2.Y) };
	case ET_Point:
		return Size({ uint32_t(_point.X), uint32_t(_point.Y) });;
	case ET_Size:
		return _size;
	default:
		return Size({ 0, 0 });
	}
}

} // End frox