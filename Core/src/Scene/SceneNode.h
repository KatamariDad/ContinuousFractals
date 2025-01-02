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
		GeometryPtr geometry,
		MaterialPtr material )
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

	bool IsValid() const;

private:

	// relative position from parent
	Vector3f m_relativePosition;
	std::vector<SceneNode*> m_children;

	GeometryPtr m_geometry;
	MaterialPtr m_material;

	// If you call yourself a Camera just fuck my shit up fam
	friend class Camera;
};


using SceneNodePtr = std::unique_ptr<SceneNode>;
using SceneNodeSharedPtr = std::shared_ptr<SceneNode>;
