#pragma once
#include <Math/Vector2.h>

class Layer2D;
struct PixelColour;
namespace Image
{
	class Image;
}

class LayerClippingWindow
{
public:
	LayerClippingWindow(
		const Vector2f& min,
		const Vector2f& max,
		Image::Image& image );

	void ApplyColourToLocation( 
		const PixelColour& colour, 
		const Vector2f& location );

	Vector2f GetPixelSize() const;
	const Vector2f& GetMin() const { return m_min; }
	const Vector2f& GetMax() const { return m_max; }
	const float GetWidth() const { return m_max.x - m_min.x; }
	const float GetHeight() const { return m_max.y - m_min.y; }
	
private:

	Vector2f m_min;
	Vector2f m_max;
	Vector2f m_scaleConversion;
	Image::Image& m_image;
};

class Rasterizer
{
public:
	// layers are infinite planes. When we rasterize them to an image
	// a clipping window must be provided
	static void RasterizeLayerToImage(
		const Layer2D& layer,
		LayerClippingWindow& window,
		Image::Image& outImage
	);
};
