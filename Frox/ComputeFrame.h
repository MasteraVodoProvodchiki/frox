#pragma once

#include "Common.h"
#include "Types.h"

#include <memory>


namespace frox {

enum EComputeFrameType
{
	ECFT_None = 0,
	ECFT_Bool,
	ECFT_UInt8,
	ECFT_UInt16,
	ECFT_UInt32,
	ECFT_Float
};

struct ComputeFrameType
{
	EComputeFrameType Type;
	uint32_t Channels;
};

namespace utils {

uint32_t FrameTypeToSize(EComputeFrameType type);

} // End utils

class ComputeFrame
{
public:
	virtual ~ComputeFrame() {}

	virtual ComputeFrameType GetType() const = 0;
	virtual uint32_t GetChannels() const = 0;
	virtual uint32_t GetElementSize() const = 0;
	virtual bool IsValid() const = 0;
	virtual Size GetSize() const = 0;
	virtual const void* GetData() const = 0;
	virtual const void* GetRowData(uint32_t row) const = 0;
	virtual const void* At(uint32_t row, uint32_t column) const = 0;
	virtual void* GetData() = 0;
	virtual void* GetRowData(uint32_t row) = 0;
	virtual void* At(uint32_t row, uint32_t column) = 0;

	// Inlines
	template <typename T>
	const T* GetData() const;

	template <typename T>
	const T* GetRowData(uint32_t row) const;

	const void* At(const Point& point) const
	{
		return this->At(point.Y, point.X);
	}

	template <typename T>
	const T& At(uint32_t row, uint32_t column) const;

	template <typename T>
	const T& At(const Point& point) const;

	template <typename T>
	T* GetData();

	template <typename T>
	T* GetRowData(uint32_t row);

	void* At(const Point& point)
	{
		return this->At(point.Y, point.X);
	}

	template <typename T>
	T& At(uint32_t row, uint32_t column);

	template <typename T>
	T& At(const Point& point);
};
using ComputeFramePtr = std::shared_ptr<ComputeFrame>;


template <typename T>
const T* ComputeFrame::GetData() const
{
	// Check type
	return reinterpret_cast<T*>(this->GetData());
}

template <typename T>
const T* ComputeFrame::GetRowData(uint32_t row) const
{
	// Check type
	return reinterpret_cast<T*>(this->GetRowData(row));
}

template <typename T>
const T& ComputeFrame::At(uint32_t row, uint32_t column) const
{
	// Check type
	return *reinterpret_cast<T*>(this->At(row, column));
}

template <typename T>
const T& ComputeFrame::At(const Point& point) const
{
	// Check type
	return this->At(point);
}

template <typename T>
T* ComputeFrame::GetData()
{
	// Check type
	return reinterpret_cast<T*>(this->GetData());
}

template <typename T>
T* ComputeFrame::GetRowData(uint32_t row)
{
	// Check type
	return reinterpret_cast<T*>(this->GetRowData(row));
}

template <typename T>
T& ComputeFrame::At(uint32_t row, uint32_t column)
{
	// Check type
	return *reinterpret_cast<T*>(this->At(row, column));
}

template <typename T>
T& ComputeFrame::At(const Point& point)
{
	// Check type
	return this->At(point);
}

} // End frox