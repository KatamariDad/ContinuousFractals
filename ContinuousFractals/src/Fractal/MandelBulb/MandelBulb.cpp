#include "pch.h"
#include "MandelBulb.h"

#include <sstream>
#include <cmath>


void MandelBulb::GenerateColourForInput(
	const Vector3f& input,
	const FractalColourizer& colourizer,
	PixelColour& outColour ) const
{
	// https://en.wikipedia.org/wiki/Mandelbulb
	bool isInSet = true;
	uint32_t divergenceIteration = 0;
	Vector3f localInput( input );
	for( uint32_t i = 0; i < m_maxIterations; ++i )
	{
		const float r = localInput.Length();
		const float r_n = std::powf( r, m_order );
		const float theta = std::atan2f( std::sqrtf( localInput.x * localInput.x + localInput.y * localInput.y ), localInput.z );
		const float phi = std::atan2f( localInput.y, localInput.x );

		const float theta_n = theta * m_order;
		const float phi_n = phi * m_order;
		localInput.x = r_n * std::sin( theta_n ) * std::cos( phi_n );
		localInput.y = r_n * std::sin( theta_n ) * std::sin( phi_n );
		localInput.z = r_n * std::cos( theta_n );

		localInput = localInput + input;

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
		outColour );
}

std::string MandelBulb::GetFractalDesc() const
{
	std::stringstream dimensionsStream;
	dimensionsStream << "mandelblub_n=" << m_order << "_it=" << m_maxIterations;
	return dimensionsStream.str();
}
