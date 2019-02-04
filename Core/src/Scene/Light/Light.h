#pragma once

#include <Math/Vector3.h>

class Light
{
public:

	Light(
		const Vector3f& position,
		const Vector3f& colourAsVector )
		: m_position( position )
		, m_colourAsVector( colourAsVector )
	{}

	const Vector3f& GetPosition() const { return m_position; }
	const Vector3f& GetColour() const { return m_colourAsVector; }
private:

	Vector3f m_colourAsVector;
	Vector3f m_position;


};
