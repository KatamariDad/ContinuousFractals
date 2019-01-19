#include "pch.h"
#include "TimeRainbow.h"

#include <chrono>
#include <ctime>

TimeRainbow::TimeRainbow()
	: now(time(0))
{
}

void TimeRainbow::GenerateColour(
	const Vector3f& input,
	const Vector3f& functionOutput,
	const bool isInSet,
	const uint32_t divergenceIteration,
	PixelColour& outColour ) const
{
	tm ltm;
	::localtime_s(&ltm, &now);	
	int hour = ltm.tm_hour;
	int minutes = ltm.tm_min;

}
