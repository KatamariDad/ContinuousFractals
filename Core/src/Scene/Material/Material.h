#pragma once
#include <vector>

#include <Image/PixelColour.h>

class Vector3f;
class Light;
struct PixelColour;

class Material
{
public:
	virtual void ApplyMaterial(
		const Vector3f& incomingRayDirection,
		const Vector3f& hitLocation,
		const Vector3f& hitNormal,
		const std::vector<const Light*>& lights,
		PixelColour& outColour ) const
	{
		outColour = Colours::WHITE;
	}
};
