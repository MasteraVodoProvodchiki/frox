#pragma once

#include "Common.h"

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
		ET_Uint32,
		ET_Float
	};

public:
	Variant()
		: _type(ET_None)
	{}

	Variant(bool value)
		: _type(ET_Bool)
		, _bool(value)
	{}

	Variant(int value)
		: _type(ET_Int)
		, _int(value)
	{}

	Variant(uint32_t value)
		: _type(ET_Uint32)
		, _int(value)
	{}

	Variant(float value)
		: _type(ET_Float)
		, _float(value)
	{}

	bool IsValid() const
	{
		return _type != ET_None;
	}

	template<typename T>
	bool Is() const
	{
		return false;
	}

	template<typename T>
	T As() const
	{
		return T();
	}

	template<typename T>
	T To() const
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
	};
};

// Is
template<>
bool Variant::Is<bool>() const
{
	return _type == ET_Bool;
}

template<>
bool Variant::Is<uint8_t>() const
{
	return _type == ET_Int;
}

template<>
bool Variant::Is<uint16_t>() const
{
	return _type == ET_Int;
}

template<>
bool Variant::Is<int>() const
{
	return _type == ET_Int;
}

template<>
bool Variant::Is<uint32_t>() const
{
	return _type == ET_Uint32;
}

template<>
bool Variant::Is<float>() const
{
	return _type == ET_Float;
}

// As
template<>
bool Variant::As<bool>() const
{
	assert(_type == ET_Bool);
	return _bool;
}

template<>
uint8_t Variant::As<uint8_t>() const
{
	assert(_type == ET_Int);
	return uint8_t(_int);
}

template<>
uint16_t Variant::As<uint16_t>() const
{
	assert(_type == ET_Int);
	return uint16_t(_int);
}

template<>
int Variant::As<int>() const
{
	assert(_type == ET_Int);
	return _int;
}

template<>
uint32_t Variant::As<uint32_t>() const
{
	assert(_type == ET_Uint32);
	return _uint32;
}

template<>
float Variant::As<float>() const
{
	assert(_type == ET_Float);
	return _float;
}

// To
template<>
bool Variant::To<bool>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool;
	case ET_Int:
		return _int > 0;
	case ET_Uint32:
		return _uint32 > 0;
	case ET_Float:
		return _float > 0;
	default:
		return false;
	}
}

template<>
uint8_t Variant::To<uint8_t>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return uint8_t(_int);
	case ET_Uint32:
		return uint8_t(_int);
	case ET_Float:
		return uint8_t(_float);
	default:
		return 0;
	}
}

template<>
uint16_t Variant::To<uint16_t>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return uint16_t(_int);
	case ET_Uint32:
		return uint16_t(_int);
	case ET_Float:
		return uint16_t(_float);
	default:
		return 0;
	}
}

template<>
int Variant::To<int>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return _int;
	case ET_Uint32:
		return int(_uint32);
	case ET_Float:
		return int(_float);
	default:
		return 0;
	}
}

template<>
uint32_t Variant::To<uint32_t>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1 : 0;
	case ET_Int:
		return uint32_t(_int);
	case ET_Uint32:
		return _uint32;
	case ET_Float:
		return uint32_t(_float);
	default:
		return 0;
	}
}

template<>
float Variant::To<float>() const
{
	switch (_type)
	{
	case ET_Bool:
		return _bool ? 1.f : 0.f;
	case ET_Int:
		return float(_int);
	case ET_Uint32:
		return float(_uint32);
	case ET_Float:
		return _float;
	default:
		return 0.f;
	}
}

} // End frox