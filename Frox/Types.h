#pragma once

namespace frox {

struct float4
{
	float X, Y, Z, W;

	float4(float value)
		: X(value), Y(value), Z(value), W(value)
	{}

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

} // End frox