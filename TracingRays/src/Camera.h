#pragma once

#include <Scene/SceneNode.h>
#include <Scene/Light/Light.h>
#include <Image/Image.h>

class Camera
{
public:
	Camera(
		// What to render
		SceneNode* root,
		// Viewing parameters
		const Vector3f& eye,
		const Vector3f& view,
		const Vector3f& up,
		double fovy,
		// Lighting parameters
		const Vector3f& ambient,
		const std::vector<const Light *>& lights )
		: m_root( root )
		, m_eye( eye )
		, m_view( view )
		, m_up( up )
		, m_fovy( fovy )
		, m_ambient( ambient )
		, m_lights( lights )
	{}

	void Render( Image::Image& image );

	const Vector3f& GetPosition() const { return m_root->m_relativePosition; }
	void SetPosition( Vector3f newPosition ) const { m_root->m_relativePosition = newPosition; }
	
	Camera& Pan( const Vector3f& offset );
private:
	SceneNode* m_root;
	Vector3f m_eye;
	Vector3f m_view;
	Vector3f m_up;
	double m_fovy;
	const Vector3f m_ambient;
	const std::vector<const Light *> m_lights;
};