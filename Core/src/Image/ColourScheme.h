#pragma once
#include <assert.h>

#include <Image/PixelColour.h>

template<size_t _NUM_COLOURS_>
class ColourScheme
{
public:
	ColourScheme( const PixelColour (&&colours)[_NUM_COLOURS_] )
	{
		// could move it
		std::memcpy( m_colours, colours, _NUM_COLOURS_ * sizeof( PixelColour ) );
	}
	ColourScheme( const PixelColour (&colours)[_NUM_COLOURS_] )
	{
		std::memcpy( m_colours, colours, _NUM_COLOURS_ * sizeof( PixelColour ) );
	}

	const PixelColour& At( size_t i ) const
	{
		assert( i < _NUM_COLOURS_ );
		return m_colours[i];
	}

	constexpr size_t Size() const { return _NUM_COLOURS_; }

private:
	PixelColour m_colours[_NUM_COLOURS_];



};


