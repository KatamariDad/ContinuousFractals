#pragma once
#include <Math/Vector3.h>
#include <vector>

class Geometry;
class Material;

class SceneNode
{
public:

	void AddChild( SceneNode& child );
	virtual bool IntersectRay(
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal,
		const Material*& hitMaterial
	) const = 0;

private:

	Geometry* m_geometry;

	// relative position from parent
	Vector3f m_relativePosition;
	std::vector<SceneNode*> m_children;


};
