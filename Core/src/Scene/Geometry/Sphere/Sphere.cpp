#include "pch.h"
#include "Sphere.h"

#include <cmath>

bool Sphere::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal ) const
{
	// test if closest point on ray to origin is within radius

	const Vector3f L = sceneOrigin - rayOrigin;
	const float dot = DotProduct( L, rayDir );
	if( dot < 0.f )
	{
		return false;
	}

	const float d2 = DotProduct( L, L ) - dot * dot;
	if( d2 > m_radiusSqr )
	{
		return false;
	}

	const float c = sqrtf( m_radiusSqr - d2 );
	float t0 = dot - c;
	float t1 = dot + c;
	if( t0 > t1 ) std::swap( t0, t1 );

	if (t0 < 0) { 
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	} 

	hitLocation = t0 * rayDir;
	hitNormal = ( hitLocation - sceneOrigin ).ComputeNormal();
	return true;
	
}