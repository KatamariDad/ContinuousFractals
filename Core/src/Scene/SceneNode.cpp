#include "pch.h"
#include "SceneNode.h"

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Geometry.h>


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
	const bool bHit = m_geometry.IntersectRay(
		m_relativePosition,
		rayOrigin,
		rayDir,
		hitLocation,
		hitNormal );

	if( bHit )
	{
		hitMaterial = &m_material;
	}

	return bHit;

}
