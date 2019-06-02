#pragma once


#include "Runnable.h"

#include <thread>

namespace frox {

class Thread
{
public:
	Thread(Runnable* runnable = nullptr);
	virtual ~Thread();

	void Start();
	void Stop();
	void Join();
	
protected:
	virtual void Run();

private:
	void ThreadRun();

private:
	Runnable* _runnable;
	std::thread _thread;
};

} // End frox