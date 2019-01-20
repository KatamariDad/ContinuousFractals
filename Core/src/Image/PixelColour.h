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


namespace Colours
{
	const PixelColour RED( 255, 0, 0 );
	const PixelColour ORANGE( 255, 175, 0 );
	const PixelColour YELLOW( 255, 255, 0 );
	const PixelColour GREEN( 0, 255, 0 );
	const PixelColour BLUE( 0, 0, 255 );
	const PixelColour PURPLE( 175, 0, 255 );
}
