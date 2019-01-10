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
#include <Windows.h>

#include "Utility/PNG/png.h"
#include "Utility/PNG/pngUtil.h"
#include "Utility/Image/Image.h"
#include "Utility/CLI/CommandLineParser.h"

#include "Fractal/MandelBox/MandelBox.h"
#include "Fractal/Colourizers/Colourizers.h"


#define _IMAGE_SIZE_ 256 

void DrawBox(
	const MandelBox& mandelBox,
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
	std::string directory = "D:\\Projects\\ContinuousFractals\\out";
	CLI::Match( "-dir", argc, argv, [&directory]( char* param ) {directory = param; } );
	directory += "\\" + CurrentTimeAndDate();

	uint32_t width = _IMAGE_SIZE_, height = _IMAGE_SIZE_;
	CLI::Match( "-img", argc, argv, [&width, &height]( char* param ) {
		sscanf_s( param, "%dx%d", &width, &height );
	} );

	MandelBox mandelBox( 2.f, 300 );
	bool drawBox = CLI::Match( "-box", argc, argv, [&mandelBox]( char* param ) {
		float formulaScale = 2.f;
		int iterationCount = 300;
		sscanf_s( param, "%f,%d", &formulaScale, &iterationCount );
		mandelBox.ResetParams( formulaScale, iterationCount );
	} );


	if (!CreateDirectoryA( directory.c_str(), NULL ))
	{
		std::cerr << "Error creating directory to save fractal output" << std::endl;
		return 1;
	}

	DrawBox( mandelBox, width, height, directory );

	return 0;
}

void DrawBox( 
	const MandelBox& mandelBox, 
	const uint32_t width, 
	const uint32_t height, 
	const std::string& dir )
{
	const std::string filename( "mandelBox" );
	const std::string extension( ".png" );

	// scale 2, max = 6
	// scale 1.89: max = 6.49, width = 15
	// scale -1.5: max = ? , width = 4, maxIt = 300
	const float minDepth = 0.000f;//6.489f;
	const float maxDepth = 2.f;
	const float increment = 0.05f;

	float currentDepth = minDepth;
	while( currentDepth <= maxDepth )
	{
		MandelBox mandelBox( -1.5f, 300 );

		const std::string directory( "D:\\Projects\\ContinuousFractals\\out\\" );
		const std::string filename( "mandelBox" );
		const std::string extension( ".png" );

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << mandelBox.GetParamDesc() << "_" << "z=" << currentDepth;
		const std::string dimensions( dimensionsStream.str() );


		const std::string fullpath = dir + "\\" + filename + dimensions + extension;
		Image::Image image( width, height, fullpath );

		SimpleColourScaledByFunctorOutputValue colourizer;
		FractalGenerator3D mandelBoxGenerator;
		FractalGenerator3D::GenerateParams params(colourizer);
		params.origin = Vector3f( 0.f, 0.f, currentDepth );
		params.scale = Vector3f( 15.0f );
		params.multithreadEnabled = true;
		mandelBoxGenerator.Generate( image, params, mandelBox );

		// write to file
		image.Save();

		currentDepth += increment;
	}
}

