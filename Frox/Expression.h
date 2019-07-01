#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "Variant.h"

#include <memory>

namespace frox {

class Expression : public std::enable_shared_from_this<Expression>
{
public:
	enum Type
	{
		ET_Frame,
		ET_Variant
	};

public:
	Expression()
	{}

	virtual ~Expression()
	{}

	// Update Or Get ??
	virtual Type GetType() const { return ET_Variant; }
	virtual bool IsValid() const { return true; }

	virtual ComputeFramePtr GetFrame() const
	{
		return nullptr;
	}

	virtual Variant GetValue() const
	{
		return Variant();
	}

	template<typename T>
	inline T GetValue() const
	{
		return T();
	}
};

template<>
inline ComputeFramePtr Expression::GetValue<ComputeFramePtr>() const
{
	return this->GetFrame();
}

using ExpressionPtr = std::shared_ptr<Expression>;

} // End frox