#include "pch.h"
#include "Image.h"

#include <fstream>
#include "PNG/png.h"


namespace Image
{

	void Image::Save() const
	{
		std::ofstream out(
			m_filename,
			std::ios::binary );
		TinyPngOut pngout( m_width, m_height, out );
		pngout.write( m_pixels, m_width * m_height );
		out.close();

	}

}
