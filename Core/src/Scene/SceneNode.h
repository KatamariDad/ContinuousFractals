#pragma once
#include <Math/Vector3.h>
#include <vector>

class Geometry;

class SceneNode
{
public:

	void AddChild( SceneNode& child );

private:

	Geometry* m_geometry;

	// relative position from parent
	Vector3f m_relativePosition;
	std::vector<SceneNode*> m_children;


};
