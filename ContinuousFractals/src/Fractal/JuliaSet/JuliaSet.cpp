#include "pch.h"
#include "JuliaSet.h"

#include <sstream>
#include <stdio.h>


void JuliaSet::GenerateColourForInput(
	const ComplexNumber& input,
	const FractalColourizer& colourizer,
	PixelColour& outColour ) const
{
	const ComplexNumber iterationOffset = 
		  m_useInputForIterationOffset 
		? input 
		: m_iterationOffset;

	bool isInSet = true;
	uint32_t divergenceIteration = 0;

	ComplexNumber z( input );
	for( uint32_t i = 0; i < m_maxIterations; ++i )
	{
		z = ComplexNumber::WholePower(z, m_order) + iterationOffset;

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
	std::string fractalName = m_useInputForIterationOffset ? "mandelbrotSet_" : "juliaSet_";
	std::stringstream dimensionsStream;
	dimensionsStream << fractalName << "n=" << m_order << "_it=" << m_maxIterations << "c = "<< m_iterationOffset.r << "+" << m_iterationOffset.i << "i";
	return dimensionsStream.str();
}