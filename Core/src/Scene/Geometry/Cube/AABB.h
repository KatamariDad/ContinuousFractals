#pragma once
#include <Scene/Geometry/Geometry.h>

class AABB final : public Geometry
{
public:

	AABB(const Vector3f& extent)
		: m_halfExtent(0.5f * extent)
	{}

	virtual bool IntersectRay(
		const Vector3f& sceneOrigin,
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal
	) const override;

private:

	Vector3f m_halfExtent;

};

