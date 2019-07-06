#pragma once

#include <functional>
#include <memory>
#include <tuple>

namespace frox {

class ComputeTask
{
public:
	ComputeTask()
		: _bCompleted(false)
	{}
	virtual ~ComputeTask() {}

	virtual bool IsValid() const = 0;
	virtual void Perform() const = 0;

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

	virtual bool IsValid() const override
	{
		return true;
	}

	virtual void Perform() const override
	{
		_functor();
	}

private:
	FunctorT _functor;
};

template <typename ValidatorT, typename FunctorT>
class TComputeTaskWithValidator : public TComputeTask<FunctorT>
{
public:
	TComputeTaskWithValidator(ValidatorT validator, FunctorT functor)
		: TComputeTask<FunctorT>(functor)
		, _validator(validator)
	{}

	virtual bool IsValid() const override
	{
		return _validator();
	}

private:
	ValidatorT _validator;
};

} // End frox