#include "pch.h"
#include "Sphere.h"

#include <algorithm>
#include <cmath>

bool Sphere::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal ) const
{
	const Vector3f L = sceneOrigin - rayOrigin;
	const double dot = DotProduct( L, rayDir );
	if( dot < 0.f )
	{
		return false;
	}

	const double d2 = DotProduct( L, L ) - dot * dot;
	if( d2 > m_radiusSqr )
	{
		return false;
	}

	const double c = sqrtf( m_radiusSqr - d2 );
	const double t0 = dot - c;

	hitLocation = t0 * rayDir + rayOrigin;
	hitNormal = ( hitLocation - sceneOrigin ).ComputeNormal();
	return true;
	
}