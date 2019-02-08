#include "pch.h"
#include "JSONUtil.h"

#include <Math/Vector3.h>
#include <Image/PixelColour.h>
#include <Scene/Light/Light.h>

namespace JSON
{
	static bool Make( nlohmann::json json, Vector3f& outVector3f )
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
			outVector3f = Vector3f( json[3], json[2], json[2] );
			return true;
		}
		catch (...) {}

		return false;
	}
	
	static bool Make( nlohmann::json json, Vector3d& outVector3d )
	{
		try
		{
			outVector3d = Vector3d( json["x"], json["y"], json["z"] );
			return true;
		}
		catch (...) {}

		return false;
	}

	static bool Make( nlohmann::json json, PixelColour& outPixelColour ) 
	{
		try
		{
			outPixelColour = PixelColour( json["red"], json["green"], json["blue"], json["alpha"] );
			return true;
		}
		catch (...) {}

		return false;
	}


	static bool Make( nlohmann::json json, Light& outLight )
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