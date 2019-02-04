#pragma once
#include <Scene/Geometry/Geometry.h>

class Sphere final : public Geometry
{
public:

	Sphere( float radius )
		: m_radius( radius )
		, m_radiusSqr( radius * radius )
	{}

	virtual bool IntersectRay(
		const Vector3f& sceneOrigin,
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal
	) const override;

private:

	float m_radius;
	float m_radiusSqr;


};
