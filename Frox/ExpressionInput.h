#pragma once

#include "Expression.h"
#include "ComputeNodePin.h"
#include "FlowDataImpl.h"

namespace frox {

struct ExpressionInput : public InputPin
{
	ExpressionPtr Expr;

	ExpressionInput()
	{}

	ExpressionInput(ExpressionPtr expr)
		: Expr(expr)
	{}

	virtual void ConnectFrom(Pin* pin) override;
};

template<typename T>
struct TLazyValueByExpression
{
	ExpressionPtr Expr;
	FlowDataImplPtr Data;
	T DefaultValue;

	TLazyValueByExpression(FlowDataImplPtr expr, FlowDataImplPtr data, T defaultValue)
		: Expr(expr)
		, Data(data)
		, DefaultValue(defaultValue)
	{}

	TLazyValueByExpression(const TLazyValueByExpression& other)
		: Expr(other.Expr)
		, Data(other.Data)
		, DefaultValue(other.DefaultValue)
	{}

	T Get() const
	{
		return Expr ? Expr->GetValue<T>(Data) : DefaultValue;
	}

	T operator * () const
	{
		return Get();
	}
};

template<typename T>
struct TExpressionInput : public ExpressionInput
{
	T DefaultValue;
	
	explicit TExpressionInput(ExpressionPtr expr)
		: ExpressionInput(expr)
	{}

	template<typename ...ArgsT>
	TExpressionInput(ArgsT... args)
		: DefaultValue(args...)
	{}

	inline bool IsValid() const
	{
		return Expr ? Expr->IsValid() : true;
	}

	inline TLazyValueByExpression<T> GetValue(FlowDataImplPtr data) const
	{
		return TLazyValueByExpression<T>(Expr, data, DefaultValue);
			// Expr ? Expr->GetValue<T>(data) : DefaultValue;
	}
};

inline bool TExpressionInput<ComputeFramePtr>::IsValid() const
{
	return Expr ? Expr->IsValid() : DefaultValue != nullptr;
}

} // End frox
