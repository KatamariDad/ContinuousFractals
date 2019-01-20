#include "pch.h"
#include "TimeRainbow.h"

#include <chrono>
#include <ctime>

const float k_hoursInADay = 24.f;

TimeRainbow::TimeRainbow()
{
	time_t now( time( 0 ) );
	::localtime_s(&m_now, &now);	
	m_currentHour = m_now.tm_hour + 1;
	m_currentMinute = m_now.tm_min + 1;
	// re-align so 6am is 0
	m_currentHour = m_currentHour < 6 ? 24 - 6 : m_currentHour - 6;
}

void TimeRainbow::GenerateColour(
	const Vector3f& input,
	const Vector3f& functionOutput,
	const bool isInSet,
	const uint32_t divergenceIteration,
	PixelColour& outColour ) const
{
	// red: 6am -> 10am
	// orange: 10am -> 2pm
	// yellow: 2pm -> 6pm
	// green: 6pm -> 10pm
	// blue: 10pm -> 2am
	// purple: 2am -> 6am
	if( isInSet )
	{
		const float rainbowHourScale = static_cast<float>( m_currentHour ) / k_hoursInADay;
		Rainbow::GetRainbowAt( rainbowHourScale, outColour );
	}
	else
	{
		outColour.r = 0;
		outColour.g = 0;
		outColour.b = 0;
	}
}

const char* TimeRainbow::ToString() const { return "TimeRainbow"; }