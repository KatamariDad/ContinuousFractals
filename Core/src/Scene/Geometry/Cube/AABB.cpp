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
	const Vector3f Min = sceneOrigin - m_halfExtent;
	const Vector3f Max = sceneOrigin + m_halfExtent;

	// from https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	// r.dir is unit direction vector of ray
	Vector3f rayDirInv(0.f);
	constexpr float epsilon = std::numeric_limits<float>::epsilon();
	rayDirInv.x = 1.0f / ((abs(rayDir.x) > 0.f) ? rayDir.x : epsilon);
	rayDirInv.y = 1.0f / ((abs(rayDir.y) > 0.f) ? rayDir.y : epsilon);
	rayDirInv.z = 1.0f / ((abs(rayDir.z) > 0.f) ? rayDir.z : epsilon);
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (Min.x - rayOrigin.x) * rayDirInv.x;
	float t2 = (Max.x - rayOrigin.x) * rayDirInv.x;
	float t3 = (Min.y - rayOrigin.y) * rayDirInv.y;
	float t4 = (Max.y - rayOrigin.y) * rayDirInv.y;
	float t5 = (Min.z - rayOrigin.z) * rayDirInv.z;
	float t6 = (Max.z - rayOrigin.z) * rayDirInv.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	float t = 0.f;
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	hitLocation = (t * rayDir) + rayOrigin;

	// computing the normal - https://blog.johnnovak.net/2016/10/22/the-nim-ray-tracer-project-part-4-calculating-box-normals/
	const Vector3f D = 0.5f * (Max - Min);
	const Vector3f P = hitLocation - sceneOrigin;
	constexpr float bias = 1.00001;

	hitNormal.x = (int)((P.x / D.x) * bias);
	hitNormal.y = (int)((P.y / D.y) * bias);
	hitNormal.z = (int)((P.z / D.z) * bias);
	hitNormal.Normalize();
	return true;
}
