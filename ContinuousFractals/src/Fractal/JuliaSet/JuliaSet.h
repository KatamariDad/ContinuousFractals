#pragma once
#include <Fractal/Fractal.h>
#include <functional>


class JuliaSet final : public FractalFunctor2DComplex
{
public:

	// try different formulae! http://www.ijon.de/mathe/julia/some_julia_sets_1_en.html
	// args: PixelInput, IterationValue, C
	typedef std::function<ComplexNumber( const ComplexNumber&, const ComplexNumber&, const ComplexNumber&)> JuliaFunctor;

	static JuliaFunctor Mandelbrot;

	JuliaSet(
		const JuliaFunctor& iterationFunctor
		, const ComplexNumber& functionScalar
		, uint32_t maxIterations = 500
		, const char* functorDesc = "JuliaSet" )
		: m_iterationFunctor( iterationFunctor )
		, m_functionScalar( functionScalar )
		, m_maxIterations( maxIterations )
		, m_functorDesc( functorDesc )
	{}

	void ResetFunctorScalar(
		const ComplexNumber& scalar )
	{
			m_functionScalar = scalar;
	}

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
	ComplexNumber m_functionScalar;
	uint32_t m_maxIterations;
	std::string m_functorDesc;
};


