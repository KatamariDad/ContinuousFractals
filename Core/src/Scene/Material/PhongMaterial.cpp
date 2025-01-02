#include "pch.h"
#include "PhongMaterial.h"

#include <Scene/Light/Light.h>
#include <Image/PixelColour.h>
#include <Math/Math.h>

void PhongMaterial::ApplyMaterial(
	const Vector3f& incomingRayDirection,
	const Vector3f& hitLocation,
	const Vector3f& hitNormal,
	const std::vector<LightSharedPtr>& lights,
	PixelColour& outColour ) const
{
	// it's technically blinn-phong, u mad?
	Vector3f result;
	for (size_t i = 0; i < lights.size(); ++i)
	{
		const Vector3f L = lights[i]->GetPosition() - hitLocation;
		const float distanceToLight = L.Length();
		const Vector3f L_n = L / ( distanceToLight );

		// Diffuse
		float intensity = DotProduct( hitNormal, L_n );
		intensity = clamp( intensity, 0.f, 1.f );
		const Vector3f diffuse = ( m_diffusePower / distanceToLight ) *  intensity * m_diffuse;

		// specular
		// half vector btwn view and light vectors
		const Vector3f H = L_n - incomingRayDirection;
		const Vector3f H_n = H.IsZero() ? incomingRayDirection : H.ComputeNormal();
		float NdotH = DotProduct( hitNormal, H_n );

		NdotH = clamp( NdotH, 0.f, 1.f );
		const float specularIntensity = std::powf( NdotH, m_shininess );

		// combine specular and diffuse colours
		const Vector3f specular = specularIntensity * ( m_shininess / distanceToLight ) * m_specular;
		result = specular + diffuse + result;
	}


	result = m_ambient + result;

	result.x = clamp( result.x, 0.f, 255.f );
	result.y = clamp( result.y, 0.f, 255.f );
	result.z = clamp( result.z, 0.f, 255.f );
	outColour.r = static_cast<uint8_t>( result.x );
	outColour.g = static_cast<uint8_t>( result.y );
	outColour.b = static_cast<uint8_t>( result.z );
}

