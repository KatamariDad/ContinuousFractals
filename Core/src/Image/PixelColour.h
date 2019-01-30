#pragma once
#include <assert.h>
#include <cstdint>
#include <cmath>
#include <string>

struct PixelColour
{
	PixelColour()
		: r( 0 )
		, g( 0 )
		, b( 0 )
	{}

	PixelColour( uint8_t c )
		: r( c )
		, g( c )
		, b( c )
	{}

	PixelColour( uint8_t _r
		, uint8_t _g
		, uint8_t _b )
		: r( _r )
		, g( _g )
		, b( _b )
	{}

	PixelColour( std::string hexCode )
		: r(0)
		, g( 0 )
		, b( 0 )
	{
		if (hexCode.at( 0 ) == '#')
		{
			r = std::stoi( hexCode.substr( 1, 2 ), nullptr, 16 );
			g = std::stoi( hexCode.substr( 3, 2 ), nullptr, 16 );
			b = std::stoi( hexCode.substr( 5, 2 ), nullptr, 16 );
		}
	}

	uint8_t r;
	uint8_t g;
	uint8_t b;
};

namespace Colours
{
	const PixelColour RED( 255, 0, 0 );
	const PixelColour ORANGE( 255, 175, 0 );
	const PixelColour YELLOW( 255, 255, 0 );
	const PixelColour GREEN( 0, 255, 0 );
	const PixelColour BLUE( 0, 0, 255 );
	const PixelColour PURPLE( 175, 0, 255 );

}


class Rainbow
{
	enum eRainbow
	{
		red = 0, orange = 1, yellow = 2, green = 3, blue = 4, purple = 5, COUNT
	};

	static constexpr float k_numRainbowColours = static_cast<float>(eRainbow::COUNT);

	static PixelColour GetColourAtIndex( size_t i )
	{
		assert( i < eRainbow::COUNT );
		eRainbow r = static_cast<eRainbow>( i );
		switch( r )
		{
		case red:
		{
			return Colours::RED;
		}
		case orange:
		{
			return Colours::ORANGE;
		}
		case green:
		{
			return Colours::GREEN;
		}
		case yellow:
		{
			return Colours::YELLOW;
		}
		case blue:
		{
			return Colours::BLUE;
		}
		case purple:
		{
			return Colours::PURPLE;
		}
		default:
		{
		}
		
		return PixelColour();
		}
	}

public:
	// give the colour scaled t between left and right.
	// t = 0 returns left, t = 1 returns right
	static void Interpolate( 
		const PixelColour& left, 
		const PixelColour& right, 
		float t, PixelColour& out )
	{
		assert( t >= 0.f && t <= 1.0f );
		out.r = static_cast<uint8_t>( static_cast<float>( left.r ) * ( 1.0f - t ) + ( static_cast<float>( right.r ) * t ) );
		out.g = static_cast<uint8_t>( static_cast<float>( left.g ) * ( 1.0f - t ) + ( static_cast<float>( right.g ) * t ) );
		out.b = static_cast<uint8_t>( static_cast<float>( left.b ) * ( 1.0f - t ) + ( static_cast<float>( right.b ) * t ) );
	}

	// interpolates over a rainbow ROYGBIV-like from 0.f to 1.f
	static void GetRainbowAt( float t, PixelColour& out )
	{
		assert( t >= 0.f && t <= 1.0f );
		constexpr float colourWidth = 1.0f / k_numRainbowColours;
		const float scale_t = t / colourWidth;
		const float scale_t_floor = floorf( scale_t );
		const size_t lowColourIndex = static_cast<size_t>( scale_t_floor );
		const size_t highColourIndex = lowColourIndex + 1;
		const float midScale = scale_t - scale_t_floor;
		const PixelColour left = GetColourAtIndex( lowColourIndex );
		const PixelColour right = GetColourAtIndex( highColourIndex );
		Interpolate( left, right, midScale, out );
	}
};




