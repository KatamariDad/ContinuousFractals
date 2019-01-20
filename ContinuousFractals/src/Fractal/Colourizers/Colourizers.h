#pragma once
#include <Fractal/Fractal.h>
#include <Image/PixelColour.h>

class BlackAndWhite final : public FractalColourizer
{
public:
	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		PixelColour& outColour) const override
	{
		outColour.r = isInSet ? 255 : 0;
		outColour.g = isInSet ? 255 : 0;
		outColour.b = isInSet ? 255 : 0;
	}

	virtual const char* ToString() const { return "B&W"; }
};

class SolidColour final : public FractalColourizer
{
public:
	SolidColour():SolidColour( PixelColour( 0, 0, 0 ) ) {} // Fuck yeah c++11

	SolidColour( PixelColour colour ) : m_colour( colour ) 
	{ 
		m_name = "rgb(" + std::to_string( colour.r )
			+ "," + std::to_string( colour.g )
			+ "," + std::to_string( colour.b ) + ")";
	}


	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		PixelColour& outColour ) const override
	{
		outColour.r = isInSet ? m_colour.r : 255;
		outColour.g = isInSet ? m_colour.g : 255;
		outColour.b = isInSet ? m_colour.b : 255;
	}

	virtual const char* ToString() const { return m_name.c_str(); }
private:
	PixelColour m_colour;
	std::string m_name;

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
		PixelColour& outColour) const override
	{
		if( isInSet )
		{
			outColour.r = 255 - ( functionOutput.x > 1.0f
				? static_cast<uint8_t>( 255.f / functionOutput.x )
				: static_cast<uint8_t>( 255.f * functionOutput.x ) );
			outColour.g = 255 - ( functionOutput.y > 1.0f
				? static_cast<uint8_t>( 255.f / functionOutput.y )
				: static_cast<uint8_t>( 255.f * functionOutput.y ) );
			outColour.b = 255 - ( functionOutput.z > 1.0f
				? static_cast<uint8_t>( 255.f / functionOutput.z )
				: static_cast<uint8_t>( 255.f * functionOutput.z ) );
		}
		else
		{
			outColour.r = m_backgroundColour.r;
			outColour.g = m_backgroundColour.g;
			outColour.b = m_backgroundColour.b;
		}
	}

	virtual const char* ToString() const { return "OutputScaled"; }

	PixelColour m_backgroundColour;
};

class ShadesOfBlueColourizer final : public FractalColourizer
{
public:
	ShadesOfBlueColourizer()
		: m_backgroundColour( 0 )
	{}

	ShadesOfBlueColourizer(
		const PixelColour& backgroundColour )
		: m_backgroundColour( backgroundColour )
	{}

	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		PixelColour& outColour) const override
	{
		if (isInSet)
		{
			outColour.r = (uint8_t)((functionOutput.x > 1.0f
				? static_cast<uint8_t>(255.f / functionOutput.x)
				: static_cast<uint8_t>(255.f * functionOutput.x)) * functionOutput.z);
			outColour.g = (uint8_t)((functionOutput.y > 1.0f
				? static_cast<uint8_t>(255.f / functionOutput.y)
				: static_cast<uint8_t>(255.f * functionOutput.y)) * functionOutput.z);
			outColour.b = 255;
		}
		else
		{
			outColour.r = m_backgroundColour.r;
			outColour.g = m_backgroundColour.g;
			outColour.b = m_backgroundColour.b;
		}
	}

	PixelColour m_backgroundColour;

	virtual const char* ToString() const { return "BlueShades"; }
};

