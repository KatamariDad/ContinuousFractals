#include "pch.h"
#include "Sphere.h"

#include <Math/Math.h>

bool Sphere::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal ) const
{
	return SphereIntersectRay(sceneOrigin, m_radiusSqr, rayOrigin, rayDir, hitLocation, hitNormal);
}