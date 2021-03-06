#include "pch.h"
#include <cstdio>
#include <sstream>
#include <iostream>
#include <map>

#include "Time/Stopwatch.h"

#include "Interpolation.h"
#include "Image/PixelColour.h"
#include "Image/Image.h"

using namespace Interpolation;

void InterpolationTestRunner::Run()
{
	std::string baseFilename( m_directory );
	baseFilename += "\\InterpTest_" ;

	const int width = 1024;
	const int height = 250;
	const int gradientHeight = 50;
	
	PixelColour from( 255, 255, 255 ); // white
	PixelColour to( 0, 0, 255 ); // blue

	PixelColour black( 0, 0, 0);

	std::map<const char*, TransformFunc1D> testFunctions =
	{
		{"SmoothStart2", SmoothStart2},
		{"SmoothStart3", SmoothStart3},
		{"SmoothStart4", SmoothStart4},
		{"SmoothStart5", SmoothStart5},
		{"SmoothStop2", SmoothStop2},
		{"SmoothStop3", SmoothStop3},
		{"SmoothStop4", SmoothStop4},
		{"SmoothStop5", SmoothStop5},
		{"SmoothStartArch3", SmoothStartArch3},
		{"SmoothStopArch3", SmoothStopArch3},
		{"SmoothStepArch4", SmoothStepArch4},
		{"BellCurve6", BellCurve6}
	};

	Time::Stopwatch stopwatch;
	for (auto function : testFunctions)
	{
		stopwatch.Start();
		Image::Image image( width, height, baseFilename + function.first + ".png");
		std::cout << "Drawing: " << function.first << std::endl;

		for (int i = 0; i < width; ++i)
		{
			float t = function.second((float)i / width);
			int j = 0;
			PixelColour pixel;
			Rainbow::Interpolate( from, to, t, pixel );
			// need to flip t because 0,0 is top left but we want a graph from the bottom
			float pixelHeight = (height - gradientHeight) * Flip(t);
			for ( ; j < gradientHeight; ++j)
			{
				image.WritePixel( i, j, pixel);
			}
			for (; j < height; ++j)
			{
				if ((j-gradientHeight) == (int)pixelHeight)
				{
					image.WritePixel( i, j, black );
				}
				else
				{
					image.WritePixel( i, j, from );
				}
			}
		}

		image.Save();

		std::cout << "Done. Took " << stopwatch.Lap() << " seconds.\n\n";
	}
	
}