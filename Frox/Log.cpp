#include "Log.h"
#include "Frox.h"

#include <iostream>

using namespace std;

namespace frox {

class DefaultLogListerner : public ILogListerner
{
public:
	static const char* TypeToName(ELogType type)
	{
		switch (type)
		{
		case Info:
			return "Info";
		case Warning:
			return "Warning";
		case Error:
			return "Error";
		default:
			return "Unknown";
		}
	}

	virtual void OnFroxLog(ELogType type, const char* message, const char* space = nullptr) override
	{
		if (space != nullptr)
		{
			cout << TypeToName(type) << ": [" << space << "] " << message << std::endl;
		}
		else
		{
			cout << TypeToName(type) << ": " << message << std::endl;
		}	
	}
};

DefaultLogListerner gDefaultListerner;
ILogListerner* gLogListerner = &gDefaultListerner;

void Log::SetListerner(ILogListerner* listerner)
{
	gLogListerner = listerner != nullptr ? listerner : &gDefaultListerner;
}

void Log::Info(const char* message, const char* space)
{
	gLogListerner->OnFroxLog(ELogType::Info, message, space);
}

void Log::Warning(const char* message, const char* space)
{
	gLogListerner->OnFroxLog(ELogType::Info, message, space);
}

void Log::Error(const char* message, const char* space)
{
	gLogListerner->OnFroxLog(ELogType::Info, message, space);
}

} // End frox