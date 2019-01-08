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


#define _IMAGE_SIZE_ 256 

void DrawBox( MandelBox mandelBox, int width, int height, std::string dir );

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

	MandelBox mandelBox( 2.f, 150 );
	bool drawBox = CLI::Match( "-box", argc, argv, [&mandelBox]( char* param ) {
		float formulaScale = 2.f;
		int iterationCount = 150;
		sscanf_s( param, "%f,%d", &formulaScale, &iterationCount );

		mandelBox = MandelBox( formulaScale, iterationCount );
	} );


	if (!CreateDirectoryA( directory.c_str(), NULL ))
	{
		std::cout << "Error creating directory to save fractal output";
	}

	DrawBox( mandelBox, height, width, directory );

	return 0;
}

void DrawBox( MandelBox mandelBox, int width, int height, std::string dir )
{
	const std::string filename( "mandelBox" );
	const std::string extension( ".png" );

	const float increment = -0.0001f;
	const float rad = 6.f;
	int count = 10;
	float current = rad;
	while (current > -rad && count > 0)
	{
		count--;

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << mandelBox.GetParamDesc() << "_" << "z=" << current;
		const std::string dimensions( dimensionsStream.str() );


		const std::string fullpath = dir + "\\" + filename + dimensions + extension;
		Image::Image image( width, height, fullpath );

		FractalGenerator3D mandelBoxGenerator;
		FractalGenerator3D::GenerateParams params;
		params.Origin = Vector3f( 0.f, 0.f, current );
		params.scale = Vector3f( 15.0f );
		mandelBoxGenerator.Generate( image, params, mandelBox );

		// write to file
		image.Save();

		current += increment;
	}
}

