#pragma once
#include <Factory/Factory.h>
#include <IOManip/FileSystem.h>
#include <Time/Stopwatch.h>
#include <Fractal/Colourizers/ColourizerFactory.h>
#include <Time/TimeUtil.h>

#include "Fractal.h"
#include "Fractal/MandelBox/MandelBox.h"
#include "Fractal/MandelBulb/MandelBulb.h"
#include "Fractal/JuliaSet/JuliaSet.h"


template<>
class Factory<JuliaSet::JuliaFunctor>
{
public:
	virtual std::unique_ptr<JuliaSet::JuliaFunctor> Build( const std::string& key, const nlohmann::json& params )
	{
		// Try out different formulas!!!
		if (key == "mandelbrot")
		{
			return std::unique_ptr<JuliaSet::JuliaFunctor>( new JuliaSet::JuliaFunctor(JuliaSet::Mandelbrot) );
		}
		else if (key == "donut")
		{
			// c = -0.18 -> 0.18 is valid range
			return std::unique_ptr<JuliaSet::JuliaFunctor>( 
				new JuliaSet::JuliaFunctor( []( 
					const ComplexNumber& input, 
					const ComplexNumber& iteration, 
					const ComplexNumber& c )
				{
					const ComplexNumber z_3 = ComplexNumber::WholePower( iteration, 3 );
					return (z_3 - c) / iteration;
				}
			));
		}
		else if (key == "donut2") // idk how 2 name things sry mybad
		{
			// c = (-.45 + -.45i) -> (0.45 + 0.45i) is valid range
			return std::unique_ptr<JuliaSet::JuliaFunctor>(
				new JuliaSet::JuliaFunctor( [](
					const ComplexNumber& input, 
					const ComplexNumber& z, 
					const ComplexNumber& c )
				{
					const ComplexNumber z_5 = ComplexNumber::WholePower( z, 5 );
					const ComplexNumber z_3 = ComplexNumber::WholePower( z, 2 );
					return (z_5 + c) / z;
				}
			));
		}
		else if (key == "donut3") // idk how 2 name things sry mybad
		{
			// c = (-.08 -.08i) -> (0.08 + 0.08i) is valid range
			return std::unique_ptr<JuliaSet::JuliaFunctor>(
				new JuliaSet::JuliaFunctor( [](
					const ComplexNumber& input,
					const ComplexNumber& z,
					const ComplexNumber& c )
				{
					const ComplexNumber z_5 = ComplexNumber::WholePower( z, 5 );
					const ComplexNumber z_3 = ComplexNumber::WholePower( z, 2 );
					return (z_5 + c) / z_3;
				}
			));
		}
		else if (key == "thereIsNoGod")
		{
			// valid from like (-1 + 1i) -> (1 + 1i)? idk this one's weird.
			return std::unique_ptr<JuliaSet::JuliaFunctor>(
				new JuliaSet::JuliaFunctor( [](
					const ComplexNumber& input,
					const ComplexNumber& z,
					const ComplexNumber& c )
				{
					const ComplexNumber z_3 = ComplexNumber::WholePower( z, 3 );
					return (z_3 + c) / (z_3 - c);
				}
			));
		}
		return std::unique_ptr<JuliaSet::JuliaFunctor>( new JuliaSet::JuliaFunctor( JuliaSet::Mandelbrot ) );
	}
};

template<>
class Factory<FractalGenerator>
{
public:
	// Draws a fractal
	// DOESN'T ACTUALLY BUILD AND RETURN A GENERATOR
	virtual std::unique_ptr<FractalGenerator> Build( const std::string& key, const nlohmann::json& params )
	{
		Factory<FractalColourizer> ColourizerFactory;
		const std::string colourizerName = params["colourizer"];
		std::unique_ptr<FractalColourizer> colourizer = ColourizerFactory.Build( colourizerName, params );
		if (key == "MandelBox")
		{
			MandelBox mandelBox( params["formulaScale"], params["iterationCount"] );
			DrawBox( mandelBox, params, *colourizer );
			return nullptr;
		}
		else if (key == "JuliaSet")
		{
			Factory<JuliaSet::JuliaFunctor> JuliaFunctorFactory;
			std::string functionName = params["functionName"];
			std::unique_ptr<JuliaSet::JuliaFunctor> functor = JuliaFunctorFactory.Build( functionName, params );
			JuliaSet julia( *functor, ComplexNumber( params["scalarMin"]["r"], params["scalarMin"]["i"] ), params["iterationCount"] );
			DrawJulia( julia, params, *colourizer );
			return nullptr;
		}

		return nullptr;
	}
private:
	static void DrawBox(
		const MandelBox& mandelBox,
		const nlohmann::json& params,
		const FractalColourizer& colourizer );
	
	static void DrawJulia( 
		JuliaSet& juliaSet, 
		const nlohmann::json& params, 
		const FractalColourizer& colourizer );

};

