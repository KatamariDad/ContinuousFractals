#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>
#include <cstdio>
#include <Windows.h>

#include <Image/PNG/png.h>
#include <Image/PNG/pngUtil.h>
#include <Image/Image.h>
#include <Image/GIF/GifUtil.h>
#include <IOManip/CommandLineParser.h>
#include <IOManip/json.hpp>
#include <Math/Interpolation.h>
#include <Time/Stopwatch.h>
#include <Time/TimeUtil.h>

#include <Fractal/JuliaSet/JuliaSet.h>
#include <Fractal/MandelBulb/MandelBulb.h>
#include <Fractal/MandelBox/MandelBox.h>
#include <Fractal/Colourizers/ColourizerFactory.h>

void DrawBox( 
	const MandelBox& mandelBox,
	FractalColourizer& colourizer,
	const uint32_t width, 
	const uint32_t height, 
	const float minDepth,
	const float maxDepth,
	const float increment,
	const std::string& baseDir )
{
	//MandelBulb mandelBox(4, 500);
	ComplexNumber juliaIterationOffset( 0.379998f, 0.2f );
	JuliaSet::JuliaFunctor f = 
		[&juliaIterationOffset]
		( const ComplexNumber& i, const ComplexNumber& z, const ComplexNumber& scalar ) 
		{ return ComplexNumber::WholePower( z, 2 ) + scalar; };

	const std::string extension( ".png" );
	const std::string directory = baseDir + "\\" + mandelBox.GetFractalDesc() + "_" + colourizer.ToString();
	if (!CreateDirectoryA(directory.c_str(), NULL ))
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
	}
	
	// scale 2, max = 6
	// scale 1.89: max = 6.49, width = 15
	// scale -1.5: max = ? , width = 4, maxIt = 300
	//const float minDepth = 0.0;// -2.0;
	//const float maxDepth = 0.41;
	//const float increment = 0.01f;

	uint32_t imageIdx = 0;

	float currentDepth = minDepth;
	const float totalDepthReciprocal = 100.f / ( ( maxDepth - minDepth != 0.f ? maxDepth - minDepth : 0.f ) );
	Time::Stopwatch stopwatch;
	stopwatch.Start();
	const std::string gifName = directory + "\\fractal.gif";
	Image::Gif giraffe( gifName.c_str(), width, height );
	while( currentDepth <= maxDepth )
	{
		const Vector3f center( 0.f, 0.0f, currentDepth );
		const Vector3f scale( 2.5f );
		// LOL so we captured this var by ref above so it totes safely modifies our function hahaha
		juliaIterationOffset = ComplexNumber( currentDepth, 0.2f );

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << imageIdx << "_" << "c=(" << center.x << "," << center.y << "," << center.z << ")" << "imgS="<< scale.x;
		const std::string dimensions( dimensionsStream.str() );


		const std::string fullpath = directory + "\\" + dimensions + extension;
		Image::Image image( width, height, fullpath );

		FractalGenerator mandelBoxGenerator;
		FractalGenerator::GenerateParams params(colourizer);
		params.origin = center;
		params.scale = scale;
		params.multithreadEnabled = true;
		mandelBoxGenerator.Generate( image, params, mandelBox );

		giraffe.AddFrame( image );

		// write to file
		image.Save();

		const float timeElapsed = stopwatch.Lap();
		std::cout << "z = " << currentDepth << " (" << (currentDepth - minDepth) * totalDepthReciprocal << "%) Done. ";
		std::cout << " - " << timeElapsed << " s / " << stopwatch.TotalTime() << " s\n";
		
		currentDepth += increment;
		++imageIdx;
	}
	giraffe.Save();
	stopwatch.Stop();
}

void DrawJulia(
	JuliaSet& juliaSet,
	const ComplexNumber& scalarMin,
	const ComplexNumber& scalarMax,
	const ComplexNumber& scalarIncrement,
	FractalColourizer& colourizer,
	const uint32_t width,
	const uint32_t height,
	const std::string& baseDir )
{
	const std::string extension( ".png" );
	const std::string directory = baseDir + "\\" + juliaSet.GetFractalDesc() + "_" + colourizer.ToString();
	if( !CreateDirectoryA( directory.c_str(), NULL ) )
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
	}
	const std::string gifName = directory + "\\julia.gif";

	Image::Gif giraffe( gifName.c_str(), width, height );
	uint32_t imageIdx = 0;
	ComplexNumber currentScalar = scalarMin;
	while( currentScalar < scalarMax )
	{
		juliaSet.ResetFunctorScalar( currentScalar );

		const Vector3f center( 0.f, 0.0f, 0.f );
		const Vector3f scale( 4.5f );

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << imageIdx << "_" << "c=" << currentScalar << "imgS=" << scale.x;
		const std::string dimensions( dimensionsStream.str() );

		const std::string fullpath = directory + "\\" + dimensions + extension;
		Image::Image image( width, height, fullpath );

		FractalGenerator mandelBoxGenerator;
		FractalGenerator::GenerateParams params( colourizer );
		params.origin = center;
		params.scale = scale;
		params.multithreadEnabled = true;
		mandelBoxGenerator.Generate( image, params, juliaSet );

		// write to file
		image.Save();
		giraffe.AddFrame( image );

		++imageIdx;
		currentScalar = currentScalar + scalarIncrement;
	}

	giraffe.Save();

}

JuliaSet::JuliaFunctor SelectFunctor( const std::string& name )
{
	// Try out different formulas!!!
	if( name == "mandelbrot" )
	{
		return JuliaSet::Mandelbrot;
	}
	else if( name == "donut" )
	{
		// c = -0.18 -> 0.18 is valid range
		return []
		( const ComplexNumber& input, const ComplexNumber& iteration, const ComplexNumber& c )
		{
			const ComplexNumber z_3 = ComplexNumber::WholePower( iteration, 3 );
			return ( z_3 - c ) / iteration;
		};
	}
	else if( name == "donut2" ) // idk how 2 name things sry mybad
	{
		// c = (-.45 + -.45i) -> (0.45 + 0.45i) is valid range
		return []
		( const ComplexNumber& input, const ComplexNumber& z, const ComplexNumber& c )
		{
			const ComplexNumber z_5 = ComplexNumber::WholePower( z, 5 );
			const ComplexNumber z_3 = ComplexNumber::WholePower( z, 2 );
			return ( z_5 + c ) / z;
		};
	}
	else if( name == "donut3" ) // idk how 2 name things sry mybad
	{
		// c = (-.08 -.08i) -> (0.08 + 0.08i) is valid range
		return []
		( const ComplexNumber& input, const ComplexNumber& z, const ComplexNumber& c )
		{
			const ComplexNumber z_5 = ComplexNumber::WholePower( z, 5 );
			const ComplexNumber z_3 = ComplexNumber::WholePower( z, 2 );
			return ( z_5 + c ) / z_3;
		};
	}
	else if( name == "thereIsNoGod" )
	{
		// valid from like (-1 + 1i) -> (1 + 1i)? idk this one's weird.
		return []
		( const ComplexNumber& input, const ComplexNumber& z, const ComplexNumber& c )
		{
			const ComplexNumber z_3 = ComplexNumber::WholePower( z, 3 );
			return ( z_3 + c ) / ( z_3 - c );
		};
	}
	return JuliaSet::Mandelbrot;
}



int main( int argc, char* argv[] )
{
	using json = nlohmann::json;

	std::ifstream i( "config.json" );
	json config;
	i >> config;

	std::string directory = config["globals"]["OUTPUT_DIR"];
	directory += "\\" + Time::CurrentTimeAndDate();

	uint32_t width = config["globals"]["IMAGE_WIDTH"], height = config["globals"]["IMAGE_HEIGHT"];

	if (!CreateDirectoryA( directory.c_str(), NULL ))
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
		return 1;
	}

	Factory<FractalColourizer> ColourizerFactory;

	// YOLO NO INPUT ERROR HANDLING
	auto fractals = config["fractals"];
	for (auto& i : fractals)
	{
		const std::string& colourizerName = i["colourizer"];
		std::unique_ptr<FractalColourizer> colourizer = ColourizerFactory.Build( colourizerName, i );

		if (i["name"] == "MandelBox")
		{
			MandelBox mandelBox( i["formulaScale"], i["iterationCount"] );
			const float minDepth = i["minDepth"];
			const float maxDepth = i["maxDepth"];
			const float increment = i["increment"];
			std::cout << "================================" << std::endl;
			std::cout << mandelBox.GetFractalDesc() << std::endl;
			std::cout << "Min " << minDepth << " Max:" << maxDepth << " Increment: " << increment << std::endl;
			DrawBox( 
				mandelBox, 
				*colourizer, 
				//1024, 1024,
				//2048, 2048,
				width, 
				height, 
				minDepth,
				maxDepth,
				increment,
				directory );
			std::cout << "================================" << std::endl;
		}
		else if (i["name"] == "JuliaSet")
		{
			std::cout << "================================" << std::endl;
			std::cout << "DRAWING BOX: " << i["iterationCount"] << "," << i["formulaScale"] << std::endl;
			JuliaSet julia( SelectFunctor( i["functionName"] ), ComplexNumber(i["scalarMin"]["r"], i["scalarMin"]["i"]), i["iterationCount"] );
			DrawJulia( 
				julia, 
				ComplexNumber(i["scalarMin"]["r"], i["scalarMin"]["i"]),
				ComplexNumber(i["scalarMax"]["r"], i["scalarMax"]["i"]),
				ComplexNumber(i["scalarIncrement"]["r"], i["scalarIncrement"]["i"]),
				*colourizer, 
				width, 
				height, 
				directory );
			std::cout << "================================" << std::endl;
		}
	}

	return 0;
}