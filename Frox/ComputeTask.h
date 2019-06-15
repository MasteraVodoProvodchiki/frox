#pragma once

#include <functional>
#include <memory>

namespace frox {

class ComputeTask
{
public:
	ComputeTask()
		: _bCompleted(false)
	{}
	virtual ~ComputeTask() {}
	virtual void Perform() = 0;

	void BindOnCompleted(std::function<void(ComputeTask*)> callback)
	{
		_onCompleted = callback;
	}

	void Complete()
	{
		if (_onCompleted)
		{
			_onCompleted(this);
		}

		_bCompleted = true;	
	}

	bool IsCompleted() const
	{
		return _bCompleted;
	}

private:
	bool _bCompleted;
	std::function<void(ComputeTask*)> _onCompleted;
};
using ComputeTaskPtr = std::shared_ptr<ComputeTask>;

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