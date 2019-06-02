#pragma once

#include "Common.h"

namespace frox {

class Log
{
public:
	static void SetListerner(class ILogListerner* listerner);

	static void Info(const char* message, const char* space = nullptr);
	static void Warning(const char* message, const char* space = nullptr);
	static void Error(const char* message, const char* space = nullptr);
};

} // End frox