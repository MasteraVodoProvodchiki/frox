#include "Thread.h"


namespace frox {

Thread::Thread(Runnable* runnable)
	: _runnable(runnable)
{
}

Thread::~Thread()
{}

void Thread::Start()
{
	_thread = std::thread(std::bind(&Thread::ThreadRun, this));
}

void Thread::Stop()
{}

void Thread::Join()
{
	if (_thread.joinable())
	{
		_thread.join();
	}
}

void Thread::Run()
{}

void Thread::ThreadRun()
{
	if (_runnable != nullptr)
	{
		_runnable->Run();
	}
	else
	{
		this->Run();
	}
}

} // End frox.