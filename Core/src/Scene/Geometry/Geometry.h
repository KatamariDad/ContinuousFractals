#pragma once
#include <Math/Vector3.h>

class Material;

class Geometry
{
public:

	virtual bool Intersect( const Vector3f& ray ) = 0;

};
