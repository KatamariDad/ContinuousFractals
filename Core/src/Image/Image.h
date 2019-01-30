#pragma once
#include <cstdint>
#include <string>

#include "PixelColour.h"

namespace Image
{

	class Image
	{
	public:
		Image( 
			const uint32_t width, 
			const uint32_t height,
			const std::string& filename)
			: m_width(width)
			, m_height(height)
			, m_filename(filename)
		{
			const uint32_t size = width * height * 3;
			m_pixels = new uint8_t[size];
			memset( m_pixels, 255, size );
		}

		~Image()
		{
			delete [] m_pixels;
		}

		void WritePixel(
			uint32_t x,
			uint32_t y,
			const PixelColour& colour)
		{
			const uint32_t i = y * m_width * 3 + x * 3;
			const float currentValScale = 1.0f - colour.a;
			m_pixels[i + 0] = static_cast<uint8_t>( colour.a * colour.r + m_pixels[i + 0] * currentValScale );
			m_pixels[i + 1] = static_cast<uint8_t>( colour.a * colour.g + m_pixels[i + 1] * currentValScale );
			m_pixels[i + 2] = static_cast<uint8_t>( colour.a * colour.b + m_pixels[i + 2] * currentValScale );
		}

		void Save() const;
		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

	private:
		uint32_t m_width;
		uint32_t m_height;
		uint8_t* m_pixels;
		std::string m_filename;

		friend class Gif;
	};

}

