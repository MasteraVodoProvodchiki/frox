#include "ComputeThread.h"
#include "ComputeTask.h"

#include <thread>
#include <chrono>
#include <assert.h>

namespace frox {

ComputeThread::ComputeThread()
	: _stoped(false)
{
}

ComputeThread::~ComputeThread()
{
	for (ComputeTask* task : _tasks)
	{
		delete task;
	}
}

// Runnable overrides
void ComputeThread::Run()
{
	while (!_stoped)
	{
		ComputeTask* task = Pop();
		if (task != nullptr)
		{
			task->Perform();
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			delete task;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(16));
		}
	}
}

void ComputeThread::Stop()
{
	_stoped = true;
}

void ComputeThread::Push(ComputeTask* task)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_tasks.push_back(task);
}

ComputeTask* ComputeThread::Pop()
{
	std::lock_guard<std::mutex> guard(_mutex);
	
	ComputeTask* task = nullptr;
	if (!_tasks.empty())
	{
		task = _tasks.front();
		_tasks.pop_front();
	}
	
	return task;
}

ComputeThreadPool::ComputeThreadPool(uint32_t nbThreads)
{
	assert(nbThreads > 0);

	for (uint32_t i=0;i<nbThreads;++i)
	{
		ComputeThread* thread = new ComputeThread();
		_threads.push_back(thread);
	}

	for (ComputeThread* thread : _threads)
	{
		thread->Start();
	}
}

ComputeThreadPool::~ComputeThreadPool()
{
	for (ComputeThread* thread : _threads)
	{
		thread->Stop();
	}

	for (ComputeThread* thread : _threads)
	{
		thread->Join();
		delete thread;
	}
}

ComputeThreadPool& ComputeThreadPool::Instance()
{
	static ComputeThreadPool threadPool;
	return threadPool;
}

void ComputeThreadPool::Push(ComputeTask* task)
{
	assert(!_threads.empty());
	ComputeThread* thread = _threads.front();
	thread->Push(task);
}

} // End frox