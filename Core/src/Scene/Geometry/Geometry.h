#pragma once
#include <Math/Vector3.h>

class Geometry
{
public:

	virtual bool IntersectRay(
		const Vector3f& sceneOrigin,
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal
	) const
	{
		return false;
	}
};

using GeometryPtr = std::unique_ptr<Geometry>;
using GeometrySharedPtr = std::shared_ptr<Geometry>;