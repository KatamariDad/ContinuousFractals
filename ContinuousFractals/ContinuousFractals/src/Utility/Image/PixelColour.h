#pragma once
#include <cstdint>

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

	uint8_t r;
	uint8_t g;
	uint8_t b;
};


