#include "pch.h"
#include "SceneNode.h"


void SceneNode::AddChild( 
	SceneNode& child )
{
	m_children.push_back( &child );
}

bool SceneNode::IntersectRay(
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal,
	const Material*& hitMaterial ) const
{
	const bool bHit = m_geometry->IntersectRay(
		m_relativePosition,
		rayOrigin,
		rayDir,
		hitLocation,
		hitNormal );

	if( bHit )
	{
		hitMaterial = m_material.get();
	}
	else
	{
		// no guarantee about z ordering of the children, so we've gotta
		// test them all and take the closest hit.
		Vector3f childHitLocation;
		Vector3f childHitNormal;
		const Material* childHitMaterial;
		float closestToEye = std::numeric_limits<float>::max();
		for( const auto& child : m_children )
		{
			if( child->IntersectRay(
				rayOrigin, rayDir,
				childHitLocation,
				childHitNormal,
				childHitMaterial ) )
			{
				const float distToEye = ( childHitLocation - rayOrigin ).SquaredLength();
				if( distToEye < closestToEye )
				{
					hitLocation = childHitLocation;
					hitNormal = childHitNormal;
					hitMaterial = childHitMaterial;
					closestToEye = distToEye;
				}
			}
		}
	}

	return bHit;

}
