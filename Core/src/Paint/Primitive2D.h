#pragma once
#include <Math/Vector2.h>
#include <Image/PixelColour.h>

namespace Image
{
	class Image;
}

class LayerClippingWindow;


class Primitive2D
{
public:
	virtual void Rasterize( LayerClippingWindow& window, Image::Image& outImage ) const = 0;
};

class Triangle final : public Primitive2D
{
public:
};

class Line final : public Primitive2D
{
public:
	Line( const Vector2f& p1, const Vector2f& p2, const PixelColour& colour )
		: m_p1( p1 )
		, m_p2( p2 )
		, m_width( 3 )
		, m_colour( colour )
	{
	}

	Line( const Vector2f& p1, const Vector2f& p2 )
		: m_p1( p1 )
		, m_p2( p2 )
		, m_width( 3 )
		, m_colour( Colours::BLACK )
	{
	}

	virtual void Rasterize( LayerClippingWindow& window, Image::Image& outImage ) const override;

private:
	Vector2f m_p1;
	Vector2f m_p2;
	size_t m_width;
	PixelColour m_colour;
};
