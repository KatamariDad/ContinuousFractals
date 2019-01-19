// testApp.cpp : Defines the entry point for the console application.
//
#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip> // put_time
#include <time.h>
#include <cstdio>
#include <Windows.h>

#include <PNG/png.h>
#include <PNG/pngUtil.h>
#include <Image/Image.h>
#include <IOManip/CommandLineParser.h>
#include <IOManip/json.hpp>

#include "Fractal/MandelBox/MandelBox.h"
#include "Fractal/Colourizers/Colourizers.h"


#define _IMAGE_SIZE_ 256 

void DrawBox(
	const MandelBox& mandelBox,
	FractalColourizer& colourizer,
	const uint32_t width,
	const uint32_t height,
	const std::string& dir );

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

	FractalColourizer* colourizer;
	if (CLI::Match( "blue", argc, argv, []( char* param ) {} ))
	{
		colourizer = new ShadesOfBlueColourizer();
	}
	else if(CLI::Match( "white", argc, argv, []( char* param ) {} ))
	{
		colourizer = new BlackAndWhite();
	}
	else
	{
		colourizer = new SimpleColourScaledByFunctorOutputValue();
	}

	std::ifstream i( "config.json" );
	json config;
	i >> config;

	// YOLO NO INPUT ERROR HANDLING
	auto fractals = config["fractals"];
	for (auto& i : fractals)
	{
		if (i["name"] == "MandelBox")
		{
			std::cout << "================================" << std::endl;
			std::cout << "DRAWING BOX: " << i["iterationCount"] << "," << i["formulaScale"] << std::endl;
			MandelBox mandelBox( i["formulaScale"], i["iterationCount"] );
			DrawBox( mandelBox, *colourizer, width, height, directory );
			std::cout << "================================" << std::endl;
		}
	}

	// SUCH MEMORY MANAGAMENT
	// WOW
	delete colourizer;
	return 0;
}

void DrawBox( 
	const MandelBox& mandelBox,
	FractalColourizer& colourizer,
	const uint32_t width, 
	const uint32_t height, 
	const std::string& baseDir )
{
	const std::string extension( ".png" );
	const std::string directory = baseDir + "\\MandelBox_" + mandelBox.GetParamDesc();
	if (!CreateDirectoryA(directory.c_str(), NULL ))
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
	}
	
	// scale 2, max = 6
	// scale 1.89: max = 6.49, width = 15
	// scale -1.5: max = ? , width = 4, maxIt = 300
	const float minDepth = 0.000f;//6.489f;
	const float maxDepth = 2.f;
	const float increment = 0.05f;

	float currentDepth = minDepth;
	const float totalDepthReciprocal = 100.f / ( ( maxDepth - minDepth != 0.f ? maxDepth - minDepth : 0.f ) );
	while( currentDepth <= maxDepth )
	{
		std::clock_t start = std::clock();

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << "_" << "z=" << currentDepth;
		const std::string dimensions( dimensionsStream.str() );


		const std::string fullpath = directory + "\\" + dimensions + extension;
		Image::Image image( width, height, fullpath );

		FractalGenerator3D mandelBoxGenerator;
		FractalGenerator3D::GenerateParams params(colourizer);
		params.origin = Vector3f( 0.f, 0.f, currentDepth );
		params.scale = Vector3f( 15.0f );
		params.multithreadEnabled = true;
		mandelBoxGenerator.Generate( image, params, mandelBox );

		// write to file
		image.Save();
		std::cout << "z = " << currentDepth <<" (" << (currentDepth - minDepth) * totalDepthReciprocal << "%) Done. (" << (std::clock() - start) / (double)CLOCKS_PER_SEC << " s)\n";
		currentDepth += increment;
	}
}

