#pragma once

#include "Common.h"

namespace frox {

class Log
{
public:
	static void SetListerner(class ILogListerner* listerner);

	FROX_API static void Info(const char* message, const char* space = nullptr);
	FROX_API static void Warning(const char* message, const char* space = nullptr);
	FROX_API static void Error(const char* message, const char* space = nullptr);
};

} // End frox