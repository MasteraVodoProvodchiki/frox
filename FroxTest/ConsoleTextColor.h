#pragma once

#include <inttypes.h>

class ConsoleTextColor
{
public:
	static const uint16_t ForeGroundBlue = 0x0001;
	static const uint16_t ForeGroundGreen = 0x0002;
	static const uint16_t ForeGroundLightBlue = 0x0003;
	static const uint16_t ForeGroundRed = 0x0004;
	static const uint16_t ForeGroundPurpure = 0x0005;
	static const uint16_t ForeGroundYellow = 0x0006;
	static const uint16_t ForeGroundWhite = 0x0007;
	static const uint16_t ForeGroundIntensity = 0x0008;

	static const uint16_t BackGroundBlue = 0x0010;
	static const uint16_t BackGroundGreen = 0x0020;
	static const uint16_t BackGroundRed = 0x0040;
	static const uint16_t BackGroundIntensity = 0x0080;

public:
	ConsoleTextColor(uint16_t color);
	~ConsoleTextColor();
	static void setConsoleTextColor(uint16_t color);

private:
	ConsoleTextColor()
	{}

	uint16_t _previousColor;
};