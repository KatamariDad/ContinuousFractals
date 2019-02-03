#include "pch.h"
#include "FractalFactory.h"

#include <Image/PNG/png.h>
#include <Image/PNG/pngUtil.h>
#include <Image/Image.h>
#include <Image/GIF/GifUtil.h>

#include <iostream>
#include <sstream>

void Factory<FractalGenerator>::DrawBox(
	const MandelBox& mandelBox,
	const nlohmann::json& params,
	const FractalColourizer& colourizer )
{
	const float minDepth = params["minDepth"];
	const float maxDepth = params["maxDepth"];
	const float increment = params["increment"];
	const uint32_t width = params["width"];
	const uint32_t height = params["height"];
	const std::string baseDir = params["directory"];

	const std::string extension( ".png" );
	const std::string directory = baseDir + "\\" + mandelBox.GetFractalDesc() + "_" + colourizer.ToString();
	FileSystem::MakeDirectory( directory );

	uint32_t imageIdx = 0;

	float currentDepth = minDepth;
	const float totalDepthReciprocal = 100.f / ((maxDepth - minDepth != 0.f ? maxDepth - minDepth : 0.f));
	Time::Stopwatch stopwatch;
	stopwatch.Start();
	const std::string gifName = directory + "\\fractal.gif";
	Image::Gif giraffe( gifName.c_str(), width, height );
	while (currentDepth <= maxDepth)
	{
		const Vector3f center( 0.f, 0.0f, currentDepth );
		const Vector3f scale( 2.5f );

		std::stringstream dimensionsStream;
		dimensionsStream << width << "_x_" << height << "_" << imageIdx << "_" << "c=(" << center.x << "," << center.y << "," << center.z << ")" << "imgS=" << scale.x;
		const std::string dimensions( dimensionsStream.str() );


		const std::string fullpath = directory + "\\" + dimensions + extension;
		Image::Image image( width, height, fullpath );

		FractalGenerator mandelBoxGenerator;
		FractalGenerator::GenerateParams params( colourizer );
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

void Factory<FractalGenerator>::DrawJulia( 
	JuliaSet& juliaSet, 
	const nlohmann::json& params, 
	const FractalColourizer& colourizer )
{
	const ComplexNumber scalarMin( params["scalarMin"]["r"], params["scalarMin"]["i"] );
	const ComplexNumber scalarMax( params["scalarMax"]["r"], params["scalarMax"]["i"] );
	const ComplexNumber scalarIncrement( params["scalarIncrement"]["r"], params["scalarIncrement"]["i"] );
	const uint32_t width = params["width"];
	const uint32_t height = params["height"];
	const std::string baseDir = params["directory"];
	const std::string extension( ".png" );
	const std::string directory = baseDir + "\\" + juliaSet.GetFractalDesc() + "_" + colourizer.ToString();
	FileSystem::MakeDirectory( directory );

	const std::string gifName = directory + "\\julia.gif";

	Image::Gif giraffe( gifName.c_str(), width, height );
	uint32_t imageIdx = 0;
	ComplexNumber currentScalar = scalarMin;
	while (currentScalar < scalarMax)
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
