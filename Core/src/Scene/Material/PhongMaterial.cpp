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
    for (const Light* light : lights)
    {
        // Diffuse
        const Vector3f L = light->GetPosition() - hitLocation;
		const float d = L.Length(); 
        const Vector3f L_n = L / d;

		float diffuse = DotProduct( L_n, hitNormal );
        clamp(diffuse, 0.f, 1.f);

        // Specular
        const Vector3f H = ( L_n - incomingRayDirection ).ComputeNormal();
        const float nDotH = DotProduct( H, hitNormal );
        float specular = std::pow( nDotH, m_shininess );
        clamp( specular, 0.f, 1.f );

        const Vector3f& I = light->GetColour(); // * ( 1.f / ( d * 0.001f ) );
		const Vector3f result = CrossProduct( I, ( diffuse * m_diffuse + specular * m_specular ) );
		outColour.r += static_cast<uint8_t>( result.x );
		outColour.g += static_cast<uint8_t>( result.y );
		outColour.b += static_cast<uint8_t>( result.z );
    }

}

