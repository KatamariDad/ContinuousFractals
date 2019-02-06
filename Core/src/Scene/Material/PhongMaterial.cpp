#include "pch.h"
#include "PhongMaterial.h"

#include <Scene/Light/Light.h>
#include <Image/PixelColour.h>

template<typename T>
void clamp( T& f, T min, T max )
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
		const Vector3f L =  light->GetPosition() - hitLocation;
		const float distanceToLight = L.Length();
		const Vector3f L_n = L / ( distanceToLight );

		// Diffuse
		float intensity = DotProduct( hitNormal, L_n );
		clamp( intensity, 0.f, 1.f );
		const Vector3f diffuse = ( m_diffusePower / distanceToLight ) *  intensity * m_diffuse;

		// specular
		// half vector btwn view and light vectors
		const Vector3f H = L_n - incomingRayDirection;
		const Vector3f H_n = H.IsZero() ? incomingRayDirection : H.ComputeNormal();
		float NdotH = DotProduct( hitNormal, H_n );
		if( NdotH > 0.f )
		{
			printf( "" );
		
		}
		clamp( NdotH, 0.f, 1.f );
		const float specularIntensity = std::powf( NdotH, m_shininess );

		// combine specular and diffuse colours
		const Vector3f specular = specularIntensity * ( m_shininess / distanceToLight ) * m_specular;
		Vector3f result = specular + diffuse + Vector3f( outColour.r, outColour.g, outColour.b );
		clamp( result.x, 0.f, 255.f );
		clamp( result.y, 0.f, 255.f );
		clamp( result.z, 0.f, 255.f );
		outColour.r = static_cast<uint8_t>( result.x );
		outColour.g = static_cast<uint8_t>( result.y );
		outColour.b = static_cast<uint8_t>( result.z );
    }


}

