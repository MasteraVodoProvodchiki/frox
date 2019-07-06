#pragma once

#include "Expression.h"
#include "ComputeNodePin.h"
#include "FlowDataImpl.h"

namespace frox {

struct ExpressionInput : public InputPin
{
	ExpressionPtr Expr;

	ExpressionInput(const char* name)
		: InputPin(name)
	{}

	ExpressionInput(const char* name, ExpressionPtr expr)
		: InputPin(name)
		, Expr(expr)
	{}

	virtual void ConnectFrom(Pin* pin) override;
	virtual void ConnectFrom(Guid id) override;
};

template<typename T>
struct TLazyValueByExpression
{
	ExpressionPtr Expr;
	FlowDataImplPtr Data;
	T DefaultValue;

	TLazyValueByExpression(ExpressionPtr expr, FlowDataImplPtr data, T defaultValue)
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
	
	explicit TExpressionInput(const char* name, ExpressionPtr expr)
		: ExpressionInput(name, expr)
	{}

	template<typename ...ArgsT>
	TExpressionInput(const char* name, ArgsT... args)
		: ExpressionInput(name)
		, DefaultValue(args...)
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

	TExpressionInput<T>& operator = (T value)
	{
		DefaultValue = value;
		return *this;
	}
};

inline bool TExpressionInput<ComputeFramePtr>::IsValid() const
{
	return Expr ? Expr->IsValid() : DefaultValue != nullptr;
}

} // End frox
