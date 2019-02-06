#pragma once
#include <Math/Vector3.h>
#include <vector>

class Geometry;
class Material;

class SceneNode
{
public:

	SceneNode(
		const Vector3f& location,
		const Geometry& geometry,
		const Material& material )
		: m_relativePosition( location )
		, m_geometry( geometry )
		, m_material( material )
	{}

	void AddChild( SceneNode& child );

	bool IntersectRay(
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal,
		const Material*& hitMaterial ) const;

private:

	// relative position from parent
	Vector3f m_relativePosition;
	const Geometry& m_geometry;
	std::vector<SceneNode*> m_children;
	const Material& m_material;

};
