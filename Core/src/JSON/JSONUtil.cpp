#include "pch.h"
#include "JSONUtil.h"

#include <Math/Vector3.h>
#include <Image/PixelColour.h>
#include <Scene/Light/Light.h>
#include <Scene/Geometry/Geometry.h>
#include <Scene/Geometry/Sphere/Sphere.h>
#include <Scene/Material/PhongMaterial.h>

// I hate that we have to try-catch here but StackOverflow says it's free as long as we don't actually throw. 
// https://stackoverflow.com/questions/16784601/does-try-catch-block-decrease-performance
namespace JSON
{
	bool Make( nlohmann::json json, Vector3f& outVector3f )
	{
		try
		{
			outVector3f = Vector3f( json["x"], json["y"], json["z"] );
			return true;
		}
		catch (...) {}

		// Next level strats
		try
		{
			outVector3f = Vector3f( json[0], json[1], json[2] );
			return true;
		}
		catch (...) {}

		return false;
	}
	
	bool Make( nlohmann::json json, Vector3d& outVector3d )
	{
		try
		{
			outVector3d = Vector3d( json["x"], json["y"], json["z"] );
			return true;
		}
		catch (...) {}

		// Next level strats
		try
		{
			outVector3d = Vector3d( json[0], json[1], json[2] );
			return true;
		}
		catch (...) {}

		return false;
	}

	bool Make( nlohmann::json json, PixelColour& outPixelColour ) 
	{
		try
		{
			float alpha = 1.f;
			try
			{
				alpha = json["alpha"];
			}
			catch (...) {}
			outPixelColour = PixelColour( json["red"], json["green"], json["blue"], alpha );
			return true;
		}
		catch (...) {}

		// Next level strats
		try
		{
			float alpha = 1.f;
			try
			{
				alpha = json[3];
			}
			catch (...) {}

			outPixelColour = PixelColour( json[0], json[1], json[2], alpha );
			return true;
		}
		catch (...) {}

		return false;
	}


	bool Make( nlohmann::json json, Light& outLight )
	{
		try
		{
			Vector3f colour;
			Vector3f position;

			JSON::Make( json["position"], position );
			JSON::Make( json["colour"], colour );

			outLight = Light(position, colour);
			return true;
		}
		catch (...) {}

		return false;
	}

}