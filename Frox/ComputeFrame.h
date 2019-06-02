#pragma once

#include "Common.h"

#include <memory>
#include <inttypes.h>

namespace frox {

enum EComputeFrameType
{
	ECFT_None = 0,
	ECFT_UInt8,
	ECFT_Float
};

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
	virtual bool IsValid() const = 0;
	virtual Size GetSize() const = 0;
	virtual void* GetData() const = 0;
};
using ComputeFramePtr = std::shared_ptr<ComputeFrame>;

} // End frox