#pragma once

#include "Common.h"

#include <memory>
#include <inttypes.h>

namespace frox {

enum EComputeFrameType
{
	ECFT_None = 0,
	ECFT_UInt8,
	ECFT_UInt16,
	ECFT_UInt32,
	ECFT_Float
};

namespace utils {

uint32_t FrameTypeToSize(EComputeFrameType type);

} // End utils

struct Size
{
	uint32_t Width;
	uint32_t Height;
};

class ComputeFrame
{
public:
	virtual ~ComputeFrame() {}

	virtual EComputeFrameType GetType() const = 0;
	virtual uint32_t GetElementSize() const = 0;
	virtual bool IsValid() const = 0;
	virtual Size GetSize() const = 0;
	virtual const void* GetData() const = 0;
	virtual const void* GetRowData(uint32_t row) const = 0;
	virtual void* GetData() = 0;
	virtual void* GetRowData(uint32_t row) = 0;

	template <typename T>
	const T* GetData() const
	{
		// Check type
		return reinterpret_cast<T*>(this->GetData());
	}

	template <typename T>
	const T* GetRowData(uint32_t row) const
	{
		// Check type
		return reinterpret_cast<T*>(this->GetRowData(row));
	}

	template <typename T>
	T* GetData()
	{
		// Check type
		return reinterpret_cast<T*>(this->GetData());
	}

	template <typename T>
	T* GetRowData(uint32_t row)
	{
		// Check type
		return reinterpret_cast<T*>(this->GetRowData(row));
	}
};
using ComputeFramePtr = std::shared_ptr<ComputeFrame>;

} // End frox