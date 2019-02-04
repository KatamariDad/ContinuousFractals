#pragma once
#include <Math/Vector3.h>
#include <Scene/SceneNode.h>

class Material;

class Geometry
{
public:

	virtual bool IntersectRay(
		const Vector3f& sceneOrigin,
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal
	) const = 0;
};
