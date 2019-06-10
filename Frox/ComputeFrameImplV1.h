#pragma once

#include "ComputeFrame.h"

namespace frox {

struct MatData
{
	union
	{
		uint8_t Uint8;
		uint16_t Uint16;
		uint32_t Uint32;
		float Scalar;
		void* Data = nullptr;
		size_t Mem;
	};

	uint32_t Width = 0;
	uint32_t Height = 0;
	EComputeFrameType Type = ECFT_None;
	bool Valid = false;

	bool IsOptimized() const
	{
		return Width == 1 && Height == 1;
	}
};

/**
 * SImple ComputeFrame with MatData
 */
class ComputeFrameImplV1 : public ComputeFrame
{
public:
	ComputeFrameImplV1(Size size = Size{ 0, 0 }, EComputeFrameType type = ECFT_None, void* data = nullptr);
	virtual ~ComputeFrameImplV1() override;

	static ComputeFramePtr Create(Size size = Size{ 0, 0 }, EComputeFrameType type = ECFT_None, void* data = nullptr)
	{
		return std::make_shared<ComputeFrameImplV1>(size, type, data);
	}

	// ComputeFrame overrides
	virtual EComputeFrameType GetType() const override;
	virtual uint32_t GetElementSize() const override;
	virtual bool IsValid() const override;
	virtual Size GetSize() const override;
	virtual const void* GetData() const override;
	virtual const void* GetRowData(uint32_t row) const override;
	virtual void* GetData() override;
	virtual void* GetRowData(uint32_t row) override;

private:
	MatData _data;
};

} // End frox