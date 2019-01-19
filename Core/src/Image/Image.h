#pragma once
#include <cstdint>
#include <string>

#include <Image/PixelColour.h>

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
			m_pixels = new uint8_t[width * height * 3];
		}

		~Image()
		{
			delete [] m_pixels;
		}

		void WritePixel(
			uint32_t x,
			uint32_t y,
			uint8_t R,
			uint8_t G,
			uint8_t B )
		{
			m_pixels[y * m_width * 3 + x * 3 + 0] = R;
			m_pixels[y * m_width * 3 + x * 3 + 1] = G;
			m_pixels[y * m_width * 3 + x * 3 + 2] = B;
		}

		void WritePixel(
			uint32_t x,
			uint32_t y,
			const PixelColour& colour)
		{
			m_pixels[y * m_width * 3 + x * 3 + 0] = colour.r;
			m_pixels[y * m_width * 3 + x * 3 + 1] = colour.g;
			m_pixels[y * m_width * 3 + x * 3 + 2] = colour.b;
		}

		void Save() const;
		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

	private:
		uint32_t m_width;
		uint32_t m_height;
		uint8_t* m_pixels;
		std::string m_filename;
	};

}

