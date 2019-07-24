#pragma once

#include "kari.hpp"

#include <tuple>

struct TestContext
{
	template<typename FuncT>
	bool operator ()(FuncT f) const
	{
		return f;
	}
};

template<typename ...ArgsT>
struct TTestContext
{
	std::tuple<ArgsT...> _args;
	TTestContext(ArgsT... args)
		: _args(args...)
	{}

	template<typename FuncT>
	bool Call(FuncT f) const
	{
		return f();
	}

	template<typename FuncT, typename FirstT>
	bool CallFunction(FuncT f, FirstT first) const
	{
		return first(f, TestContext());
	}

	template<typename FuncT, typename FirstT, typename ...ArgsT>
	bool CallFunction(FuncT f, FirstT first, ArgsT... args) const
	{
		TTestContext<ArgsT...> context(args...);
		return first(f, context);
	}

	template<typename FuncT, std::size_t... Idx>
	bool CallTuple(FuncT f, const std::tuple<ArgsT...>& t, std::index_sequence<Idx...>) const
	{
		return CallFunction(f, std::get<Idx>(t)...);
	}

	template<typename FuncT>
	bool operator ()(FuncT f) const
	{
		return CallTuple(f, _args, std::index_sequence_for<ArgsT...>{});
	}
};

template<typename FuncT, typename ...ArgsT>
void generationTest(FuncT func, ArgsT... args)
{
	auto curryFunc = kari::curry(func);

	TTestContext<ArgsT...> context(args...);
	context(curryFunc);
}

/*
template<int> // begin with 0 here!
struct placeholder_template
{};

#include <functional>
#include <type_traits>

namespace std
{
	template<int N>
	struct is_placeholder< placeholder_template<N> >
		: integral_constant<int, N + 1> // the one is important
	{};
}

template<class Ret, class... Args, int... Is>
void my_bind(Ret(*p)(Args...), int_sequence<Is...>)
{
	auto x = std::bind(p, placeholder_template<Is>{}...);
	x(Args(42)...);
}

template<class Ret, class... Args>
void my_bind(Ret(*p)(Args...))
{
	my_bind(p, make_int_sequence< sizeof...(Args) >{});
}
*/



// func(type, op, size)
// bind(func, _1, _2, _3)
// test(Name, func, AllTypes, AllOperation, AllSizes)

/*
AllTypes{
	AllOperation {
		AllSizes {
			func(..., type, op, size)
		}
	}
}
*/

/*
AllTypes([](type) {
	AllOperation([](op) {
		AllSizes([](size) {
			func(..., type, op, size)
		})
	})
})
*/

/*
f = [func](type) {
	f = [func, type](op) {
		f = [func, type, op](size) {
			f = [func, type, op, size]() {
				func(type, op, size)
			}
			f()
		}
		AllSizes(f)
	}

	AllOperation(f)
}
AllTypes(f)
*/

/*
template<typename FunctionT>
void testProps(const char* name, FunctionT func)
{
	test("Add", func);
}

template<typename FunctionT, typename FirstT, typename ...ArgsT>
void testProps(const char* name, FunctionT func, FirstT arg, ArgsT ...args)
{
	auto func = std::bind(func, _1);
	auto new Func std::bind(func, _1, _2, _3, _4, type);
	arg(func);

	testProps(name, )
}
*/

/*
template<typename FuncT, typename ...TypesT>
class Props
{
	std::tuple<TypesT...> _props;
	std::function<FuncT> _func;

public:
	template<typename ArgT>
	bool operator() (ArgT arg)
	{
		_func(arg, _props...);
	}
};

template<typename FunctionT, typename FirstT, typename ...ArgsT>
void testProps(const char* name, FunctionT func, FirstT arg, ArgsT ...args)
{
	Props<> props;
	auto func = Props(func, ,_1);
	auto new Func std::bind(func, _1, _2, _3, _4, type);
	func = testProps(name, )

	arg(func);

}
*/