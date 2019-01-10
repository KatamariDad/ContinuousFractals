#pragma once
#include <Fractal/Fractal.h>
#include <Utility/Image/PixelColour.h>

class BlackAndWhite final : public FractalColourizer
{
public:
	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		uint8_t& r,
		uint8_t& g,
		uint8_t& b ) const override
	{
		r = isInSet ? 255 : 0;
		g = isInSet ? 255 : 0;
		b = isInSet ? 255 : 0;
	}
};


class SimpleColourScaledByFunctorOutputValue final : public FractalColourizer
{
public:
	SimpleColourScaledByFunctorOutputValue()
		: m_backgroundColour( 0 )
	{}

	SimpleColourScaledByFunctorOutputValue( 
		  const PixelColour& backgroundColour )
		: m_backgroundColour( backgroundColour )
	{}

	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		uint8_t& r,
		uint8_t& g,
		uint8_t& b ) const override
	{
		if( isInSet )
		{
			r = 255 - ( functionOutput.x > 1.0f
				? static_cast<uint8_t>( 255.f / functionOutput.x )
				: static_cast<uint8_t>( 255.f * functionOutput.x ) );
			g = 255 - ( functionOutput.y > 1.0f
				? static_cast<uint8_t>( 255.f / functionOutput.y )
				: static_cast<uint8_t>( 255.f * functionOutput.y ) );
			b = 255 - ( functionOutput.z > 1.0f
				? static_cast<uint8_t>( 255.f / functionOutput.z )
				: static_cast<uint8_t>( 255.f * functionOutput.z ) );
		}
		else
		{
			r = m_backgroundColour.r;
			g = m_backgroundColour.g;
			b = m_backgroundColour.b;
		}
	}

	PixelColour m_backgroundColour;
};

