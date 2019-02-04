#pragma once

#include <Scene/Material/Material.h>
#include <Math/Vector3.h>

class PhongMaterial final : public Material
{
public:
	virtual void ApplyMaterial(
		const Vector3f& incomingRayDirection,
		const Vector3f& hitLocation,
		const Vector3f& hitNormal,
		const std::vector<const Light*>& lights,
		PixelColour& outColour ) const override;


	Vector3f m_diffuse;
	Vector3f m_specular;
	// Material shininess constant.  Larger positive values model surfaces that
	// are smoother or mirror-like.  Smaller positive values model rougher surfaces.
	size_t m_shininess;
};

