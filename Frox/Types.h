#pragma once

#include <inttypes.h>
#include <limits>
#include <cmath>

namespace frox {
namespace utils {

template<typename T>
_inline bool Equal(T left, T right)
{
	return left == right;
}

template<>
_inline bool Equal<float>(float left, float right)
{
	return fabsf(left - right) <= std::numeric_limits<float>::epsilon();
}

} // End utils

struct int2
{
	int32_t X, Y;
};

struct int3
{
	int32_t X, Y, Z;
};

struct int4
{
	int32_t X, Y, Z, W;
};

struct uint2
{
	uint32_t X, Y;
};

struct uint3
{
	uint32_t X, Y, Z;
};

struct uint4
{
	uint32_t X, Y, Z, W;
};

struct float2
{
	float X, Y;

	static bool Equal(const float2& left, const float2& right)
	{
		return
			utils::Equal(left.X, right.X) &&
			utils::Equal(left.Y, right.Y);
	}
};

struct float3
{
	float X, Y, Z;

	static bool Equal(const float3& left, const float3& right)
	{
		return
			utils::Equal(left.X, right.X) &&
			utils::Equal(left.Y, right.Y) &&
			utils::Equal(left.Z, right.Z);
	}
};

struct float4
{
	float X, Y, Z, W;

	float4(float value)
		: X(value), Y(value), Z(value), W(value)
	{}

	static bool Equal(const float4& left, const float4& right)
	{
		return
			utils::Equal(left.X, right.X) &&
			utils::Equal(left.Y, right.Y) &&
			utils::Equal(left.Z, right.Z) &&
			utils::Equal(left.W, right.W);
	}

	friend bool operator < (const float4& left, const float4& right)
	{
		return
			left.X < right.X &&
			left.Y < right.Y &&
			left.Z < right.Z &&
			left.W < right.W;
	}

	friend bool operator <= (const float4& left, const float4& right)
	{
		return
			left.X <= right.X &&
			left.Y <= right.Y &&
			left.Z <= right.Z &&
			left.W <= right.W;
	}

	friend bool operator > (const float4& left, const float4& right)
	{
		return
			left.X > right.X &&
			left.Y > right.Y &&
			left.Z > right.Z &&
			left.W > right.W;
	}

	friend bool operator >= (const float4& left, const float4& right)
	{
		return
			left.X >= right.X &&
			left.Y >= right.Y &&
			left.Z >= right.Z &&
			left.W >= right.W;
	}

	template <typename OutT>
	friend OutT& operator << (OutT& out, const float4& v)
	{
		out << v.X << v.Y << v.Z << v.W;
		return out;
	}
};

struct Point
{
	int32_t X;
	int32_t Y;

	friend bool operator == (const Point& left, const Point& right)
	{
		return left.X == right.X && left.Y == right.Y;
	}

	friend bool operator != (const Point& left, const Point& right)
	{
		return left.X != right.X || left.Y != right.Y;
	}
};

struct Size
{
	uint32_t Width;
	uint32_t Height;

	bool IsValid() const
	{
		return Width > 0 && Height > 0;
	}

	friend bool operator == (const Size& left, const Size& right)
	{
		return left.Width == right.Width && left.Height == right.Height;
	}

	friend bool operator != (const Size& left, const Size& right)
	{
		return left.Width != right.Width || left.Height != right.Height;
	}
};

struct Rect
{
	int32_t X, Y, Width, Height;

	Rect()
		: X(-1), Y(-1), Width(-1), Height(-1)
	{}

	Rect(int32_t x, int32_t y, int32_t width, int32_t height)
		: X(x), Y(y), Width(width), Height(height)
	{}

	Rect(const Point& offset, const Size& size)
		: X(offset.X), Y(offset.Y), Width(int32_t(size.Width)), Height(int32_t(size.Height))
	{}

	bool IsValid() const
	{
		return X >= 0 && Y >= 0 && Width > 0 && Height > 0;
	}
};

} // End frox