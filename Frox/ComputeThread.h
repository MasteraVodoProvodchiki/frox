#pragma once

#include "Thread.h"
#include "ComputeTask.h"

#include <vector>
#include <deque>
#include <mutex>
#include <atomic>

namespace frox {

class ComputeThread : public Thread
{
public:
	ComputeThread();
	virtual ~ComputeThread();

	// Runnable overrides
	virtual void Run() override;

	void Stop();
	void Push(ComputeTaskPtr task);
	ComputeTaskPtr Pop();

private:
	std::atomic_bool _stoped;
	std::mutex _mutex;
	std::deque<ComputeTaskPtr> _tasks;
};

class ComputeThreadPool
{
	ComputeThreadPool(uint32_t nbThreads = 1);
	~ComputeThreadPool();

public:
	static ComputeThreadPool& Instance();

	void Push(ComputeTaskPtr task);

private:
	std::vector<ComputeThread*> _threads;
};

} // End frox