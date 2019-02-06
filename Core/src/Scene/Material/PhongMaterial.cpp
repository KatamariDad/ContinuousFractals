#include "pch.h"
#include "PhongMaterial.h"

#include <Scene/Light/Light.h>
#include <Image/PixelColour.h>

void clamp( float& f, float min, float max )
{
	f = f < min ? min : f;
	f = f > max ? max : f;
}

void PhongMaterial::ApplyMaterial(
	const Vector3f& incomingRayDirection,
	const Vector3f& hitLocation,
	const Vector3f& hitNormal,
	const std::vector<const Light*>& lights,
	PixelColour& outColour ) const
{
	// it's technically blinn-phong, u mad?
    for (const Light* light : lights)
    {
		const Vector3f L = light->GetPosition() - hitLocation;
		const float dist2 = L.SquaredLength();
		const float dist = sqrtf( dist2 );
		const Vector3f L_n = L / ( dist );

		// Diffuse
		float intensity = DotProduct( hitNormal, L_n );
		clamp( intensity, 0.f, 1.f );
		const Vector3f diffuse = ( static_cast<float>( m_shininess ) / dist ) *  intensity * m_diffuse;

		// specular
		// half vector btwn view and light vectors
		const Vector3f H = ( L_n + incomingRayDirection ).ComputeNormal();
		float NdotH = DotProduct(hitNormal, H);
		clamp( NdotH, 0.f, 1.f );
		const float specularIntensity = std::powf( NdotH, m_shininess );

		// combine specular and diffuse colours
		const Vector3f specular = specularIntensity * ( static_cast<float>( m_shininess ) / dist ) * m_specular;
		const Vector3f result = specular + diffuse;
		outColour.r += static_cast<uint8_t>( result.x );
		outColour.g += static_cast<uint8_t>( result.y );
		outColour.b += static_cast<uint8_t>( result.z );
    }

}

