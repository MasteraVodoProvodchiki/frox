#pragma once

namespace frox {

class ComputeTask
{
public:
	virtual ~ComputeTask() {}
	virtual void Perform() = 0;
};

template <typename FunctorT>
class TComputeTask : public ComputeTask
{
public:
	TComputeTask(FunctorT functor)
		: _functor(functor)
	{}

	virtual void Perform() override
	{
		_functor();
	}

private:
	FunctorT _functor;
};

class ComputeTaskUtils
{
public:
	template<typename FunctorT>
	static TComputeTask<FunctorT>* Make(FunctorT functor)
	{
		return new TComputeTask<FunctorT>(functor);
	}
};

} // End frox