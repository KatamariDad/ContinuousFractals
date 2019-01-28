#pragma once
#include <Fractal/Fractal.h>
#include <functional>


class JuliaSet final : public FractalFunctor2DComplex
{
public:

	// args: PixelInput, IterationValue
	typedef std::function<ComplexNumber( const ComplexNumber&, const ComplexNumber& )> JuliaFunctor;

	static JuliaFunctor Mandelbrot;

	JuliaSet(
		const JuliaFunctor& iterationFunctor
		, uint32_t maxIterations
		, const char* functorDesc = "JuliaSet" )
		: m_iterationFunctor( iterationFunctor )
		, m_maxIterations( maxIterations )
		, m_functorDesc( functorDesc )
	{}

	void ResetParams(
		const JuliaFunctor& iterationFunctor,
		uint32_t maxIterations )
	{
		m_iterationFunctor = iterationFunctor;
		m_maxIterations = maxIterations;
	}

	virtual void GenerateColourForInput(
		const ComplexNumber& Input,
		const FractalColourizer& colourizer,
		PixelColour& outColour ) const override;

	virtual std::string GetFractalDesc() const override;

private:

	JuliaFunctor m_iterationFunctor;
	uint32_t m_maxIterations;
	std::string m_functorDesc;
};


