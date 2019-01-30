#include "pch.h"
#include "JuliaSet.h"

#include <sstream>
#include <stdio.h>

namespace 
{
	ComplexNumber MandelbrotFunctor( 
		const ComplexNumber& functionInput,
		const ComplexNumber& iterationValue,
		const ComplexNumber& scalar )
	{
		return ( iterationValue * iterationValue ) + functionInput;
	}
}

JuliaSet::JuliaFunctor JuliaSet::Mandelbrot = MandelbrotFunctor;

void JuliaSet::GenerateColourForInput(
	const ComplexNumber& input,
	const FractalColourizer& colourizer,
	PixelColour& outColour ) const
{
	bool isInSet = true;
	uint32_t divergenceIteration = 0;

	ComplexNumber z( input );
	for( uint32_t i = 0; i < m_maxIterations; ++i )
	{
		z = m_iterationFunctor( input, z, m_functionScalar );

		if( z.SquaredLength() > 4.f )
		{
			isInSet = false;
			divergenceIteration = i;
		}
	}

	colourizer.GenerateColour(
		Vector3f( input.r, input.i, 0.f ),
		Vector3f( z.r, z.i, 0.f ),
		isInSet,
		divergenceIteration,
		outColour );
}

std::string JuliaSet::GetFractalDesc() const
{
	std::stringstream dimensionsStream;
	dimensionsStream << m_functorDesc << "_it=" << m_maxIterations;
	return dimensionsStream.str();
}