#pragma once

#include "json.hpp"

class Vector3f;
class Vector3d;
struct PixelColour;
class Light;

namespace JSON
{
	static bool Make( nlohmann::json json, Vector3f& outVector3f );
	static bool Make( nlohmann::json json, Vector3d& outVector3d );
	static bool Make( nlohmann::json json, PixelColour& outPixelColour );
	static bool Make( nlohmann::json json, Light& outLight );
}