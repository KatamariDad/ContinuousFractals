#pragma once
#include <Math/Vector3.h>
#include <vector>

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Geometry.h>

class SceneNode
{
public:

	SceneNode(
		const Vector3f& location,
		std::unique_ptr<Geometry> geometry,
		std::unique_ptr<Material> material )
		: m_relativePosition( location )
		, m_geometry( std::move(geometry) )
		, m_material( std::move(material) )
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
	std::vector<SceneNode*> m_children;

	std::unique_ptr<Geometry> m_geometry;
	std::unique_ptr<Material> m_material;

	// If you call yourself a Camera just fuck my shit up fam
	friend class Camera;
};
