#pragma once

#include <Scene/Material/Material.h>
#include <Math/Vector3.h>

class PhongMaterial final : public Material
{
public:

	PhongMaterial(
		const Vector3f& diffuse,
		const Vector3f& specular,
		const Vector3f& ambient,
		float shininess,
		float diffusePower) :
		m_diffuse( diffuse ),
		m_specular( specular ),
		m_ambient( ambient ),
		m_shininess( shininess ),
		m_diffusePower( diffusePower )
	{}

	static std::string GetJsonTypeName() { return "phong"; }

	virtual void ApplyMaterial(
		const Vector3f& incomingRayDirection,
		const Vector3f& hitLocation,
		const Vector3f& hitNormal,
		const std::vector<std::shared_ptr<Light>>& lights,
		PixelColour& outColour ) const override;

	Vector3f m_diffuse;
	Vector3f m_specular;
	Vector3f m_ambient;
	// Material shininess constant.  Larger positive values model surfaces that
	// are smoother or mirror-like.  Smaller positive values model rougher surfaces.
	float m_shininess;
	float m_diffusePower;
};

