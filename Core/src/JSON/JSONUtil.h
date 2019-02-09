#pragma once

#include "json.hpp"

class Vector3f;
class Vector3d;
struct PixelColour;
class Light;

namespace JSON
{
	bool Make( nlohmann::json json, Vector3f& outVector3f );
	bool Make( nlohmann::json json, Vector3d& outVector3d );
	bool Make( nlohmann::json json, PixelColour& outPixelColour );
	bool Make( nlohmann::json json, Light& outLight );
}