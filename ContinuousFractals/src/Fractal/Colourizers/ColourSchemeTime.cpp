#include "pch.h"
#include "ColourSchemeTime.h"

#include <Image/ColourScheme.h>

#include <chrono>
#include <ctime>

const float k_hoursInADay = 24.f;


// night time: blue, yellow, purple, grey, pink
const ColourScheme<5> k_colourSchemeSlot1( 
	{ PixelColour( 36, 79, 91 )
	, PixelColour( 247, 233, 128 )
	, PixelColour( 35, 15, 66 )
	, PixelColour( 194, 196, 166 )
	, PixelColour( 230, 95, 122 ) } 
); 

// all cool colours, white: 
const ColourScheme<5> k_colourSchemeSlot2( 
	{ PixelColour( 232, 232, 232 )
	, PixelColour( 191, 191, 191)
	, PixelColour( 150, 147, 155 )
	, PixelColour( 61, 47, 58 )
	, PixelColour( 54, 175, 111 ) } 
); 

// bright blue and red and cream
const ColourScheme<5> k_colourSchemeSlot3( 
	{ PixelColour( 191, 44, 0)
	, PixelColour( 41, 151, 155) 
	, PixelColour( 61, 47, 58 )
	, PixelColour( 54, 175, 111 ) 
	, PixelColour( 242, 214, 101 ) } 
); 

ColourSchemeTime::ColourSchemeTime()
{
	time_t now( time( 0 ) );
	::localtime_s(&m_now, &now);	
	m_currentHour = m_now.tm_hour + 1;
	m_currentMinute = m_now.tm_min + 1;
	// re-align so 6am is 0
	m_currentHour = m_currentHour < 6 ? 24 - 6 : m_currentHour - 6;
}

void ColourSchemeTime::GenerateColour(
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
		const float norm = functionOutput.SquaredLength();
		if( norm < 0.15 )
		{
			outColour = k_colourSchemeSlot3.At( 0 );
		}
		else if( norm < 0.35 )
		{
			outColour = k_colourSchemeSlot3.At( 1 );
		}
		else if( norm < 0.55 )
		{
			outColour = k_colourSchemeSlot3.At( 2 );
		}
		else if( norm < 1 )
		{
			outColour = k_colourSchemeSlot3.At( 3 );
		}
		else
		{
			outColour = k_colourSchemeSlot3.At( 4 );
		}
	}
	else
	{
		outColour = PixelColour( 232 );
		//const float norm = input.SquaredLength() / 100.f;
		//const PixelColour p1( 45, 25, 41 );
		//const PixelColour p2( 15, 5, 11 );
		//Rainbow::Interpolate(p2, p1, norm < 1.0f ? norm : 1.0f / norm, outColour);
	}
}

const char* ColourSchemeTime::ToString() const { return "ColourSchemeTime"; }