#pragma once

#include "Thread.h"

#include <vector>
#include <deque>
#include <mutex>
#include <atomic>

namespace frox {

class ComputeTask;
class ComputeThread : public Thread
{
public:
	ComputeThread();
	virtual ~ComputeThread();

	// Runnable overrides
	virtual void Run() override;

	void Stop();
	void Push(ComputeTask* task);
	ComputeTask* Pop();

private:
	std::atomic_bool _stoped;
	std::mutex _mutex;
	std::deque<ComputeTask*> _tasks;
};

class ComputeThreadPool
{
	ComputeThreadPool(uint32_t nbThreads = 1);
	~ComputeThreadPool();

public:
	static ComputeThreadPool& Instance();

	void Push(ComputeTask* task);

private:
	std::vector<ComputeThread*> _threads;
};

} // End frox