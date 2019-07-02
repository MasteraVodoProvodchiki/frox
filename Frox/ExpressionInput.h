#pragma once

#include "Expression.h"

namespace frox {

struct ExpressionInput
{
	ExpressionPtr Expr;

	ExpressionInput()
	{}

	ExpressionInput(ExpressionPtr expr)
		: Expr(expr)
	{}
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

	inline T GetValue() const
	{
		return Expr ? Expr->GetValue<T>() : DefaultValue;
	}
};

inline bool TExpressionInput<ComputeFramePtr>::IsValid() const
{
	return Expr ? Expr->IsValid() : DefaultValue != nullptr;
}

} // End frox
