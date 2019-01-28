#pragma once
#include <vector>
#include "../Image.h"
#include "gif.h"

namespace Image
{
	class Gif
	{
	public:
		Gif( const char* filename, uint32_t width, uint32_t height, float fps = 15.f ) 
			: m_filename( filename ) 
			, m_width(width)
			, m_height(height)
		{
			m_delay = 100.f / fps;
		};

		// Don't add an image with a size less than the gif's!
		void AddFrame( const Image& image )
		{
			int size = m_width * m_height * 4;
			std::vector<uint8_t> frame( size );
			int i = 0;
			for (int j = 0; j < size; )
			{
				frame[j++] = image.m_pixels[i++];
				frame[j++] = image.m_pixels[i++];
				frame[j++] = image.m_pixels[i++];
				frame[j++] = 0;
			}
			m_frames.push_back( frame );
		}

		void Save()
		{
			GifWriter* writer = nullptr; 
			GifBegin( writer, m_filename, m_width, m_height, m_delay);
			for (auto frame : m_frames)
			{
				GifWriteFrame( writer, frame.data(), m_width, m_height, m_delay );
			}
			GifEnd( writer );
		}
	private:
		std::vector<std::vector<uint8_t>> m_frames;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_delay;
		const char* m_filename;
	};
}