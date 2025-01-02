#include "pch.h"
#include <fstream>
#include <iostream>

#include <JSON/json.hpp>
#include <IOManip/FileSystem.h>
#include <Fractal/FractalFactory.h>


int main( int argc, char* argv[] )
{
	using json = nlohmann::json;

	std::ifstream i( "config.json" );
	json config;
	i >> config;

	std::string directory = config["globals"]["OUTPUT_DIR"];
	directory += "\\" + Time::CurrentTimeAndDate();

	uint32_t width = config["globals"]["IMAGE_WIDTH"], height = config["globals"]["IMAGE_HEIGHT"];

	FileSystem::MakeDirectory( directory );

	Factory<FractalGenerator> FractalFactory;

	// YOLO NO INPUT ERROR HANDLING
	auto fractals = config["fractals"];
	for (auto& fractalParams : fractals)
	{
		if (fractalParams.find("disable") != fractalParams.end())
		{
			continue;
		}

		// Fill in global params
		fractalParams["directory"] = directory;
		fractalParams["width"] = width;
		fractalParams["height"] = height;

		std::string fractalName = fractalParams["name"];
		std::cout << "Drawing Fractal: " << fractalName << " ...\n";
		FractalFactory.Build( fractalName, fractalParams );
		std::cout << std::endl << fractalName << " - Fractal Done.\n\n";
	}

	return 0;
}