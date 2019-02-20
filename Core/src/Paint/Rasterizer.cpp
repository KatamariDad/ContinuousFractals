#include "pch.h"
#include "Rasterizer.h"
#include <cassert>

#include <Image/Image.h>
#include <Image/PixelColour.h>
#include <Paint/Layer.h>
#include <Paint/Primitive2D.h>


LayerClippingWindow::LayerClippingWindow(
	const Vector2f& min,
	const Vector2f& max,
	Image::Image& image)
	: m_min( min )
	, m_max( max )
	, m_image( image )
{
	assert( m_min.x < m_max.x );
	assert( m_min.y < m_max.y );

	// compute min/max 
	const float imageWidth = static_cast<float>( image.GetWidth() );
	const float imageHeight = static_cast<float>( image.GetHeight() );

	m_scaleConversion = Vector2f( 
		( m_max.x - m_min.x ) / imageWidth, 
		( m_max.y - m_min.y ) / imageHeight );
}

void LayerClippingWindow::ApplyColourToLocation(
	const PixelColour& colour,
	const Vector2f& location )
{
	const float imageWidth = static_cast<float>( m_image.GetWidth() );
	const float imageHeight = static_cast<float>( m_image.GetHeight() );

	const Vector2f clampedLayerLocation(
		location.x < m_min.x ? m_min.x : ( location.x > m_max.x ? m_max.x : location.x ),
		location.y < m_min.y ? m_min.y : ( location.x > m_max.y ? m_max.y : location.y ) );

	// convert to pixel space.
	const float scaleX = ( clampedLayerLocation.x - m_min.x ) / ( m_max.x - m_min.x );
	const uint32_t pixelX = static_cast<uint32_t>( scaleX * imageWidth );

	const float scaleY = ( clampedLayerLocation.y - m_min.y ) / ( m_max.y - m_min.y );
	const uint32_t pixelY = imageHeight - static_cast<uint32_t>( scaleY * imageHeight );

	m_image.WritePixel( pixelX, pixelY, colour );
}

Vector2f LayerClippingWindow::GetPixelSize() const
{
	const float imageWidth = static_cast<float>( m_image.GetWidth() );
	const float imageHeight = static_cast<float>( m_image.GetHeight() );

	return Vector2f( 
		( m_max.x - m_min.x ) / imageWidth, 
		( m_max.y - m_min.y ) / imageHeight );
}

void Rasterizer::RasterizeLayerToImage(
	const Layer2D& layer,
	LayerClippingWindow& window,
	Image::Image& outImage )
{
	for( auto& primitive : layer.m_primitives )
	{
		primitive->Rasterize( window, outImage );
	}
}
