#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "Variant.h"
#include "FlowDataImpl.h"

#include <memory>

namespace frox {

class Expression : public std::enable_shared_from_this<Expression>
{
public:
	enum Type
	{
		ET_Any = 0,
		ET_Frame,
		ET_Variant
	};

public:
	Expression()
	{}

	virtual ~Expression()
	{}

	// Update Or Get ??
	virtual Type GetType() const { return ET_Any; }
	virtual bool IsValid() const { return true; }

	virtual ComputeFramePtr GetFrame(FlowDataImplPtr data) const
	{
		return nullptr;
	}

	virtual Variant GetValue(FlowDataImplPtr data) const
	{
		return Variant();
	}

	template<typename T>
	inline T GetValue(FlowDataImplPtr data) const
	{
		Variant value = this->GetValue(data);
		return value.To<T>();;
	}
};

template<>
inline ComputeFramePtr Expression::GetValue<ComputeFramePtr>(FlowDataImplPtr data) const
{
	return this->GetFrame(data);
}

using ExpressionPtr = std::shared_ptr<Expression>;

} // End frox