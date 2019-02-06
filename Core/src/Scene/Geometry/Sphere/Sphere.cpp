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
	const float t0 = dot - c;
	//float t0 = std::min( dot - c, dot + c );
	//if (t0 < 0.f) 
	//{ 
	//	return false;
	//} 

	hitLocation = t0 * rayDir + rayOrigin;
	hitNormal = ( hitLocation - sceneOrigin ).ComputeNormal();
	return true;
	
}