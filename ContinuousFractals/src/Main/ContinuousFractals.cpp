#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
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

#include <Fractal/JuliaSet/JuliaSet.h>
#include <Fractal/MandelBulb/MandelBulb.h>
#include <Fractal/MandelBox/MandelBox.h>
#include <Fractal/Colourizers/Colourizers.h>
#include <Fractal/Colourizers/ColourSchemeTime.h>
#include <Fractal/Colourizers/TimeRainbow.h>


#define _IMAGE_SIZE_ 256 

FractalColourizer* GetColourizerFromFractalSettings( nlohmann::json colourizerName );

std::string CurrentTimeAndDate()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t( now );

	time_t t = time( NULL );
	struct tm buf;
	::localtime_s( &buf, &t );
	char str[26];
	::asctime_s( str, sizeof str, &buf );
	for (char& c : str)
	{
		if (c == ':')
		{
			c = '-';
		}
	}
	std::string ret( str );
	return ret.substr( 0, ret.size() - 1 );
}



void DrawBox( 
	const MandelBox& mandel,
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
	JuliaSet mandelBox( f, juliaIterationOffset, 500 );

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
		size_t size = width * height * 4;
		std::vector<uint8_t> frame(size);
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
	ComplexNumber currentScalar = scalarMin;
	while( currentScalar < scalarMax )
	{
		juliaSet.ResetFunctorScalar( currentScalar );
		currentScalar = currentScalar + scalarIncrement;

		const std::string extension( ".png" );
		const std::string directory = baseDir + "\\" + juliaSet.GetFractalDesc() + "_" + colourizer.ToString();
		if( !CreateDirectoryA( directory.c_str(), NULL ) )
		{
			std::cerr << "Error creating directory to save fractal output" << std::endl;
		}

		size_t size = width * height * 4;
		std::vector<uint8_t> frame( size );
		const Vector3f center( 0.f, 0.0f, 0.f );
		const Vector3f scale( 2.5f );

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << 0 << "_" << "c=(" << center.x << "," << center.y << "," << center.z << ")" << "imgS=" << scale.x;
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
	}

}


FractalColourizer* GetColourizerFromFractalSettings( nlohmann::json fractal )
{
	try
	{
		std::string colourizerName = fractal.at( "colourizer" );
		if (colourizerName == "OutputScale")
		{
			return new SimpleColourScaledByFunctorOutputValue();
		}
		else if (colourizerName == "BlueShades")
		{
			return new ShadesOfBlueColourizer();
		}
		else if (colourizerName == "TimeRainbow")
		{
			return new TimeRainbow();
		}
		else
		{
			return new BlackAndWhite();
		}
	}
	catch (...){} // Catch block executes twice even if we return? Thanks C++

	try 
	{
		std::string colour = fractal.at( "colour" );
		if (colour.at( 0 ) == '#')
		{
			PixelColour hexColour(
				std::stoi( colour.substr( 1, 2 ), nullptr, 16 ),
				std::stoi( colour.substr( 3, 2 ), nullptr, 16 ),
				std::stoi( colour.substr( 5, 2 ), nullptr, 16 )
			);

			return new SolidColour( hexColour );
		}
		else 
		{
			return new SolidColour();
		}
	}catch(...){}

	return new BlackAndWhite();
}

JuliaSet::JuliaFunctor SelectFunctor( const std::string& name )
{
	// Try out different formulas!!!
	if( name == "mandelbrot" )
	{
		return JuliaSet::Mandelbrot;
	}
	else if( name == "circleThing" )
	{
		// c = -0.18 -> 0.18 is valid range
		return []
		( const ComplexNumber& input, const ComplexNumber& iteration, const ComplexNumber& c )
		{
			const ComplexNumber z_3 = ComplexNumber::WholePower( iteration, 3 );
			return ( z_3 - c ) / iteration;
		};
	}

	return JuliaSet::Mandelbrot;
}



int main( int argc, char* argv[] )
{
	using json = nlohmann::json;
	std::string directory = "D:\\Projects\\ContinuousFractals\\out";
	CLI::Match( "-dir", argc, argv, [&directory]( char* param ) {directory = param; } );
	directory += "\\" + CurrentTimeAndDate();

	uint32_t width = _IMAGE_SIZE_, height = _IMAGE_SIZE_;
	CLI::Match( "-img", argc, argv, [&width, &height]( char* param ) {
		sscanf_s( param, "%dx%d", &width, &height );
	} );

	if (!CreateDirectoryA( directory.c_str(), NULL ))
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
		return 1;
	}

	CLI::Match( "-interpTest", argc, argv, [&directory]( char* param ) {
		Interpolation::InterpolationTestRunner test( directory.c_str() );
		test.Run();
	} );

	std::ifstream i( "config.json" );
	json config;
	i >> config;


	// YOLO NO INPUT ERROR HANDLING
	auto fractals = config["fractals"];
	for (auto& i : fractals)
	{
		FractalColourizer* colourizer(GetColourizerFromFractalSettings(i));

		if (i["name"] == "MandelBox")
		{
			std::cout << "================================" << std::endl;
			std::cout << "DRAWING BOX: " << i["iterationCount"] << "," << i["formulaScale"] << std::endl;
			MandelBox mandelBox( i["formulaScale"], i["iterationCount"] );
			const float minDepth = i["minDepth"];
			const float maxDepth = i["maxDepth"];
			const float increment = i["increment"];
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
				ComplexNumber(i["scalarMin"]["r"], i["scalarMax"]["i"]),
				ComplexNumber(i["scalarMax"]["r"], i["scalarMax"]["i"]),
				ComplexNumber(i["scalarIncrement"]["r"], i["scalarIncrement"]["i"]),
				*colourizer, 
				width, 
				height, 
				directory );
			std::cout << "================================" << std::endl;
		}

		// SUCH MEMORY MANAGAMENT
		// WOW
		delete colourizer;
	}

	return 0;
}