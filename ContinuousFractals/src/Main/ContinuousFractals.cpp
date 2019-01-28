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

#include <PNG/png.h>
#include <PNG/pngUtil.h>
#include <Image/Image.h>
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

void DrawBox(
	const MandelBox& mandelBox,
	FractalColourizer& colourizer,
	const uint32_t width,
	const uint32_t height,
	const std::string& dir );

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
			DrawBox( 
				mandelBox, 
				*colourizer, 
				//1024, 1024,
				//2048, 2048,
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

void DrawBox( 
	const MandelBox& mandelBox,
	FractalColourizer& colourizer,
	const uint32_t width, 
	const uint32_t height, 
	const std::string& baseDir )
{
	//MandelBulb mandelBox(4, 500);
	//JuliaSet mandelBox( 2, ComplexNumber( -0.5f, 0.2f ), 500, false );

	const std::string extension( ".png" );
	const std::string directory = baseDir + "\\" + mandelBox.GetFractalDesc() + "_" + colourizer.ToString();
	if (!CreateDirectoryA(directory.c_str(), NULL ))
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
	}
	
	// scale 2, max = 6
	// scale 1.89: max = 6.49, width = 15
	// scale -1.5: max = ? , width = 4, maxIt = 300
	const float minDepth = -1;// 0.379998;// -2.0;
	const float maxDepth = 1; // 0.41;
	const float increment = 0.01f;
	uint32_t imageIdx = 0;

	float currentDepth = minDepth;
	const float totalDepthReciprocal = 100.f / ( ( maxDepth - minDepth != 0.f ? maxDepth - minDepth : 0.f ) );
	Time::Stopwatch stopwatch;
	stopwatch.Start();
	while( currentDepth <= maxDepth )
	{
		//mandelBox.ResetParams( 2, ComplexNumber( currentDepth, 0.2f ), 100 );
		const Vector3f center( 0.f, 0.5f, currentDepth );
		const Vector3f scale( 2.5f );

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

		// write to file
		image.Save();

		const float timeElapsed = stopwatch.Lap();
		std::cout << "z = " << currentDepth << " (" << (currentDepth - minDepth) * totalDepthReciprocal << "%) Done. ";
		std::cout << " - " << timeElapsed << " s / " << stopwatch.TotalTime() << " s\n";
		
		currentDepth += increment;
		++imageIdx;
	}

	stopwatch.Stop();
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
			return new ColourSchemeTime();
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

