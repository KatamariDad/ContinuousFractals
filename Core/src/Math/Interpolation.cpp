#include <cstdio>
#include <sstream>

#include "pch.h"
#include "Interpolation.h"
#include "Image/PixelColour.h"
#include "Image/Image.h"
#include "Math/Vector3.h"

void Interpolation::InterpolationTestRunner::Run()
{
	std::string baseFilename( m_directory );
	baseFilename += "\\InterpTest_" ;

	int width = 1024;
	int height = 50;
	
	Vector3f from( 255, 255, 255 ); // white
	Vector3f to( 0, 0, 255 ); // blue

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
		Image::Image image( width, height, baseFilename + function.first + ".png");
		std::cout << "Drawing: " << function.first << std::endl;
		for (int i = 0; i < width; ++i)
		{
			float t = function.second((float)i / width);
			float r = (from.x * (1 - t)) + (to.x * t);
			float g = (from.y * (1 - t)) + (to.y * t);
			float  b = (from.z * (1 - t)) + (to.z * t);
			for (int j = 0; j < height; ++j)
			{
				image.WritePixel( i, j, (int)r, (int)g, (int)b);
			}
		}

		image.Save();
	}
	
}