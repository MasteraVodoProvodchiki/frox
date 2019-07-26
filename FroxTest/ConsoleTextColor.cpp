#include "ConsoleTextColor.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifndef _MSC_VER
const uint16_t ConsoleTextColor::ForeGroundBlue;
const uint16_t ConsoleTextColor::ForeGroundGreen;
const uint16_t ConsoleTextColor::ForeGroundLightBlue;
const uint16_t ConsoleTextColor::ForeGroundRed;
const uint16_t ConsoleTextColor::ForeGroundPurpure;
const uint16_t ConsoleTextColor::ForeGroundYellow;
const uint16_t ConsoleTextColor::ForeGroundWhite;
const uint16_t ConsoleTextColor::ForeGroundIntensity;

const uint16_t ConsoleTextColor::BackGroundBlue;
const uint16_t ConsoleTextColor::BackGroundGreen;
const uint16_t ConsoleTextColor::BackGroundRed;
const uint16_t ConsoleTextColor::BackGroundIntensity;
#endif

ConsoleTextColor::ConsoleTextColor(WORD color)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	_previousColor = info.wAttributes;
	setConsoleTextColor(color);
#endif
}

ConsoleTextColor::~ConsoleTextColor()
{
#ifdef _WIN32
	setConsoleTextColor(_previousColor);
#endif
}

void ConsoleTextColor::setConsoleTextColor(WORD color)
{
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}