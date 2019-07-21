#include "pch.h"
#include "Primitive2D.h"

#include <Math/Math.h>
#include <Image/Image.h>
#include <Paint/Rasterizer.h>

namespace {
	static const float k_infinity = std::numeric_limits<float>::infinity(); 
}

void Line::Rasterize( LayerClippingWindow& window, Image::Image& outImage ) const
{
	// compute line slope
	const Vector2f pixelSize = window.GetPixelSize();

	const float lineStartX = clamp( m_p1.x, window.GetMin().x, window.GetMax().x );
	const float lineStartY = clamp( m_p1.y, window.GetMin().y, window.GetMax().y );

	const float lineEndX = clamp( m_p2.x, window.GetMin().x, window.GetMax().x );
	const float lineEndY = clamp( m_p2.y, window.GetMin().y, window.GetMax().y );

	// parametric line equation: p1 + slopeXY * t;
	const float deltaX = lineStartX != lineEndX ? pixelSize.x / ( lineEndX - lineStartX ) : k_infinity;
	const float deltaY = lineStartY != lineEndY ? pixelSize.y / ( lineEndY - lineStartY ) : k_infinity;
	const Vector2f delta = Vector2f( deltaX < 0.f ? -deltaX : deltaX, deltaY ) * 0.1f;

	for( size_t i = 0; i < m_width; ++i )
	{
		Vector2f pos = m_p1.x < m_p2.x ? m_p1.x : m_p2.x;

		while (pos.x < m_p2.x)
		{
			window.ApplyColourToLocation( m_colour, pos );
			pos = pos + delta;
		}
	
	}
}
