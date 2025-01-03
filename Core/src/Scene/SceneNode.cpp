#include "pch.h"
#include "SceneNode.h"


void SceneNode::AddChild( 
	SceneNodePtr&& child )
{
	m_children.push_back(std::move(child));
}

bool SceneNode::IsValid() const
{
	return m_geometry != nullptr && m_material != nullptr;
}

bool SceneNode::IntersectRay(
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal,
	const Material*& hitMaterial ) const
{
	if (m_geometry == nullptr)
	{
		return false;
	}

	bool bHit = m_geometry->IntersectRay(
		m_relativePosition,
		rayOrigin,
		rayDir,
		hitLocation,
		hitNormal );

	float closestDistToCamera = std::numeric_limits<float>::max();
	if (bHit)
	{
		hitMaterial = m_material.get();
		closestDistToCamera = (hitLocation - rayOrigin).SquaredLength();
	}

	// no guarantee about z ordering of the children, so we've gotta
	// test them all and take the closest hit.
	Vector3f childHitLocation;
	Vector3f childHitNormal;
	const Material* childHitMaterial = nullptr;
	for (const SceneNodePtr& child : m_children )
	{
		if (child->IntersectRay(
			rayOrigin, rayDir,
			childHitLocation,
			childHitNormal,
			childHitMaterial ) )
		{
			bHit = true;
			const float childDistToCamera = ( childHitLocation - rayOrigin ).SquaredLength();
			if( childDistToCamera < closestDistToCamera )
			{
				hitLocation = childHitLocation;
				hitNormal = childHitNormal;
				hitMaterial = childHitMaterial;
				closestDistToCamera = childDistToCamera;
			}
		}
	}

	return bHit;
}
