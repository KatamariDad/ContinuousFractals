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
			m_delay = uint32_t(100.f / fps);
		};

		void AddFrame( const Image& image )
		{
			uint32_t size = m_width * m_height * 4;
			std::vector<uint8_t> frame( size );
			uint32_t i = 0, j = 0;
			while(i < image.m_width * image.m_height * 3 )
			{
				frame[j++] = image.m_pixels[i++];
				frame[j++] = image.m_pixels[i++];
				frame[j++] = image.m_pixels[i++];
				frame[j++] = 0;
			}
			while (j < size)
			{
				frame[j++] = 0;
			}
			m_frames.push_back( frame );
		}

		void Save()
		{
			GifWriter writer; 
			GifBegin( &writer, m_filename, m_width, m_height, m_delay);
			for (auto frame : m_frames)
			{
				GifWriteFrame( &writer, frame.data(), m_width, m_height, m_delay );
			}
			GifEnd( &writer );
		}
	private:
		std::vector<std::vector<uint8_t>> m_frames;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_delay;
		const char* m_filename;
	};
}