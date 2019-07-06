#include "ComputeThread.h"
#include "ComputeTask.h"
#include "Log.h"

#include <thread>
#include <chrono>
#include <assert.h>


namespace frox {

ComputeThread::ComputeThread()
	: _stoped(false)
{
}

ComputeThread::~ComputeThread()
{}

// Runnable overrides
void ComputeThread::Run()
{
	while (!_stoped)
	{
		ComputeTaskPtr task = Pop();
		if (task != nullptr)
		{
			if (task->IsValid())
			{
				task->Perform();
			}
			else
			{
				std::string message = "Invalid Task";
				Log::Error(message.c_str(), "ComputeThread");
			}
			
			task->Complete();
			std::this_thread::sleep_for(std::chrono::microseconds(1));
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

void ComputeThread::Push(ComputeTaskPtr task)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_tasks.push_back(task);
}

ComputeTaskPtr ComputeThread::Pop()
{
	std::lock_guard<std::mutex> guard(_mutex);
	
	ComputeTaskPtr task = nullptr;
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

void ComputeThreadPool::Push(ComputeTaskPtr task)
{
	assert(!_threads.empty());
	ComputeThread* thread = _threads.front();
	thread->Push(task);
}

} // End frox