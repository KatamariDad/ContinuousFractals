#pragma once
#include <vector>

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
		PixelColour& outColour ) const = 0;
};
