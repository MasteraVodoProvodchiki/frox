#pragma once

#include "ComputeTask.h"
#include "ExpressionInput.h"

#include <functional>
#include <memory>
#include <tuple>

namespace frox {

template<typename ...TypesT>
class TComputeTaskProps
{
	std::tuple<TypesT...> _tuple;

public:
	TComputeTaskProps(TypesT... args)
		: _tuple(std::forward<TypesT>(args)...)
	{}

	template<typename TypeT>
	static TypeT UnpackValue(TLazyValueByExpression<TypeT> lazyValue)
	{
		return *lazyValue;
	}

	template<typename TypeT>
	static TypeT UnpackValue(TypeT value)
	{
		return value;
	}


	template<typename ValidatorT, std::size_t... Is>
	static bool CallValidatorWithTuple(ValidatorT validator, const std::tuple<TypesT...>& tuple, std::index_sequence<Is...>)
	{
		return validator(UnpackValue(std::get<Is>(tuple))...);
	}

	template<typename FunctorT, std::size_t... Is>
	static void CallFunctorWithTuple(FunctorT functor, const std::tuple<TypesT...>& tuple, std::index_sequence<Is...>)
	{
		functor(UnpackValue(std::get<Is>(tuple))...);
	}

	template<typename FunctorT>
	ComputeTask* MakeTask(FunctorT functor) const
	{
		auto props = _tuple;
		auto functorInvoker = [props, functor]() {
			CallFunctorWithTuple(functor, props, std::index_sequence_for<TypesT...>());
		};

		return new TComputeTask<decltype(functorInvoker)>(functorInvoker);
	}

	template<typename ValidatorT, typename FunctorT>
	ComputeTask* MakeTask(ValidatorT validator, FunctorT functor) const
	{
		auto props = _tuple;

		auto validatorInvoker = [props, validator]() {
			return CallValidatorWithTuple(validator, props, std::index_sequence_for<TypesT...>());
		};

		auto functorInvoker = [props, functor]() {
			CallFunctorWithTuple(functor, props, std::index_sequence_for<TypesT...>());
		};

		return new TComputeTaskWithValidator<decltype(validatorInvoker), decltype(functorInvoker)>(validatorInvoker, functorInvoker);
	}
};

class ComputeTaskHelper
{
public:
	template<typename FunctorT>
	static TComputeTask<FunctorT>* Make(FunctorT functor)
	{
		return new TComputeTask<FunctorT>(functor);
	}

	template<typename ...ArgsT>
	static TComputeTaskProps<ArgsT...> UnPackProps(ArgsT... args)
	{
		return TComputeTaskProps<ArgsT...>(args...);
	}
};

} // End frox