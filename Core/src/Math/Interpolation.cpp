#include <cstdio>
#include <sstream>

#include "pch.h"
#include "Interpolation.h"
#include "Image/PixelColour.h"
#include "Image/Image.h"

void Interpolation::InterpolationTestRunner::Run()
{
	std::string baseFilename( m_directory );
	baseFilename += "\\InterpTest_" ;

	int width = 256;
	int height = 50;
	
	PixelColour from( 0, 0, 0 ); // black
	PixelColour to( 0, 0, 255 ); // blue

	using namespace Interpolation;

	std::map<const char*, InterpolationFunction> testFunctions =
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

	for (auto function : testFunctions)
	{
		Image::Image image( width, height, baseFilename + function.first);
		std::cout << "Drawing: " << function.first << std::endl;
		for (int i = 0; i < width; ++i)
		{
			float t = function.second(i / width);
			float r = (from.r * (1 - t)) + (to.r * t);
			float g = (from.g * (1 - t)) + (to.g * t);
			float  b = (from.b * (1 - t)) + (to.b * t);
			for (int j = 0; j < height; ++j)
			{
				image.WritePixel( i, j, (int)r, (int)g, (int)b);
			}
		}

		image.Save();
	}
	
}