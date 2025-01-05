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
#include "Scene/Geometry/VoxelizedShape.h"

#include <iostream>

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
			const float minDepth = params["minDepth"];
			const float maxDepth = params["maxDepth"];
			const float increment = params["increment"];
			const uint32_t width = params["width"];
			const uint32_t height = params["height"];
			std::unique_ptr<VoxelizedShape> voxelizedShape = nullptr;
			if (params.find("generateVox") != params.end() && params["generateVox"] == true)
			{
				const uint32_t sizeZ = (uint32_t)(abs(maxDepth - minDepth)) / increment;
				//voxelizedShape.reset(new VoxelizedShape(width, height, sizeZ));
			}

			MandelBox mandelBox( params["formulaScale"], params["iterationCount"] );
			DrawMandelBox( mandelBox, params, *colourizer, voxelizedShape.get() );
			return nullptr;
		}
		else if (key == "MandelBulb")
		{
			const float minDepth = params["minDepth"];

			FractalGenerator::GenerateParams generateParams(*colourizer);
			generateParams.multithreadEnabled = true;
			generateParams.origin = Vector3f(0.f, 0.f, minDepth);
			generateParams.scale = Vector3f(2.5f);

			std::unique_ptr<VoxelizedShape> voxelizedShape = nullptr;
			if (params.find("generateVox") != params.end() && params["generateVox"] == true)
			{
				const float maxDepth = params["maxDepth"];
				const float increment = params["increment"];
				const uint32_t width = params["width"];
				const uint32_t height = params["height"];

				const Vector3f topLeft(generateParams.origin.x - ( 0.5f * generateParams.scale.x ), generateParams.origin.y + ( 0.5f * generateParams.scale.y ), generateParams.origin.z );
				const Vector3f bottomRight(generateParams.origin.x + ( 0.5f * generateParams.scale.x ), generateParams.origin.y - 0.5f * generateParams.scale.y, generateParams.origin.z );
				const float incrementX = ( bottomRight.x - topLeft.x ) / static_cast<float>( width );
				const float incrementY = ( topLeft.y - bottomRight.y ) / static_cast<float>( height );

				const uint32_t sizeZ = (abs(maxDepth - minDepth)) / increment;
				voxelizedShape.reset(new VoxelizedShape(width, height, sizeZ, Vector3f(incrementX, incrementY, increment)));
			}

			MandelBulb mandelBulb( params["exponent"], params["iterationCount"] );
			DrawMandelBulb( mandelBulb, params, *colourizer, generateParams, voxelizedShape.get() );

			if (voxelizedShape != nullptr)
			{
				voxelizedShape->Save(params["directory"]);
			}
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
	static void DrawMandelBox(
		const MandelBox& mandelBox,
		const nlohmann::json& params,
		const FractalColourizer& colourizer,
		VoxelizedShape* voxelizedShape );

	static void DrawMandelBulb(
		const MandelBulb& mandelBulb,
		const nlohmann::json& params,
		const FractalColourizer& colourizer,
		FractalGenerator::GenerateParams& generateParams,
		VoxelizedShape* voxelizedShape);
	
	static void DrawJulia( 
		JuliaSet& juliaSet, 
		const nlohmann::json& params, 
		const FractalColourizer& colourizer );

};

