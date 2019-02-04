#include "pch.h"
#include "Sphere.h"


bool Sphere::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal ) const
{
	// test if closest point on ray to origin is within radius

    const Vector3f v = sceneOrigin - rayOrigin;
    float d = DotProduct(v, rayDir);
    const Vector3f closestPointOnLine = rayOrigin  + d * rayDir;

	const Vector3f perpendicularVector = closestPointOnLine - sceneOrigin;

	if( perpendicularVector.SquaredLength() <= m_radiusSqr )
	{
		hitLocation = closestPointOnLine;
		hitNormal = ( closestPointOnLine - sceneOrigin ).ComputeNormal();
		return true;
	}

	return false;
	
}