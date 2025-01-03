#include "pch.h"
#include "AABB.h"

#include <Math/Math.h>
#include <math.h>

bool AABB::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal) const
{
	return AABBIntersectRay(sceneOrigin, m_halfExtent, rayOrigin, rayDir, hitLocation, hitNormal);
}
