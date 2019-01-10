#include "pch.h"
#include "MandelBox.h"
#include <string>
#include <sstream>


std::string MandelBox::GetParamDesc() const
{
	std::stringstream dimensionsStream;
	dimensionsStream << "s=" << m_formulaScale << "_it=" << m_maxIterations;
	return dimensionsStream.str();
}

void MandelBox::BallFold( Vector3f& input ) const
{
	const float length = input.SquaredLength();
	if( length < 0.25f )
	{
		input = 4.f * input;
	}
	else if( length < 1.f )
	{
		input = input / length;
	}
}

void MandelBox::BoxFold( float& input ) const
{
	if( input > 1.f )
	{
		input = 2.f - input;
	}
	else if( input < -1.f )
	{
		input = -2.f - input;
	}
}

void MandelBox::GenerateColourForInput(
	const Vector3f& input,
	const FractalColourizer& colourizer,
	uint8_t& r,
	uint8_t& g,
	uint8_t& b ) const
{
	bool isInSet = true;
	uint32_t divergenceIteration = 0;
	Vector3f localInput( input );
	for( uint32_t i = 0; i < m_maxIterations; ++i )
	{
		BoxFold( localInput.x );
		BoxFold( localInput.y );
		BoxFold( localInput.z );
		BallFold( localInput );
		localInput = m_formulaScale * localInput + input;
	
		if( localInput.IsInfinity() ) 
		{
			divergenceIteration = i;
			isInSet = false;
			break;
		}
	}

	colourizer.GenerateColour( 
		input, 
		localInput, 
		isInSet, 
		divergenceIteration, 
		r, g, b );
}


