#pragma once
#include <Fractal/Fractal.h>


class JuliaSet final : public FractalFunctor2DComplex
{
public:
	JuliaSet(
		uint32_t exponent // glynn sets use fractional exponents
		, const ComplexNumber& iterationOffset
		, uint32_t maxIterations
		, bool useInputForIterationOffset ) // true renders the mandelbrot set.
		: m_iterationOffset( iterationOffset )
		, m_maxIterations( maxIterations )
		, m_order( exponent )
		, m_useInputForIterationOffset( useInputForIterationOffset )
	{}

	void ResetParams(
		uint32_t exponent,
		const ComplexNumber& iterationOffset,
		uint32_t maxIterations )
	{
		m_order = exponent;
		m_maxIterations = maxIterations;
		m_iterationOffset = iterationOffset;
	}

	virtual void GenerateColourForInput(
		const ComplexNumber& Input,
		const FractalColourizer& colourizer,
		PixelColour& outColour ) const override;

	virtual std::string GetFractalDesc() const override;

private:

	ComplexNumber m_iterationOffset;
	uint32_t m_maxIterations;
	uint32_t m_order;
	bool m_useInputForIterationOffset;
};


