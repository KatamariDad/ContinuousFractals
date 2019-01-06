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
		input.x = input.x / length;
		input.y = input.y / length;
		input.z = input.z / length;
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
	const Vector3f& Input,
	uint8_t& r,
	uint8_t& g,
	uint8_t& b ) const
{
	bool isInSet = true;
	Vector3f localInput( Input );
	for( uint32_t i = 0; i < m_maxIterations; ++i )
	{
		BoxFold( localInput.x );
		BoxFold( localInput.y );
		BoxFold( localInput.z );
		BallFold( localInput );
		localInput = m_formulaScale * localInput + Input;
	
		if( localInput.IsInfinity() ) 
		{
			isInSet = false;
			break;
		}
	}

	if( isInSet )
	{
		r = 255 - (localInput.x > 1.0f 
			? static_cast<uint8_t>(255.f / localInput.x) 
			: static_cast<uint8_t>(255.f * localInput.x));
		g = 255 - (localInput.y > 1.0f 
			? static_cast<uint8_t>(255.f / localInput.y) 
			: static_cast<uint8_t>(255.f * localInput.y));
		b = 255 - (localInput.z > 1.0f 
			? static_cast<uint8_t>(255.f / localInput.z) 
			: static_cast<uint8_t>(255.f * localInput.z));
		r = 255;
		g = 255;
		b = 255;
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}
}


