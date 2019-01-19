#include "pch.h"
#include "pngUtil.h"
#include "png.h"

#include <fstream>

namespace PNG
{

	void SavePNG(
		const std::string& directory,
		const std::string& filename,
		const uint8_t* pixels,
		uint32_t width,
		uint32_t height )
	{
		std::ofstream out(
			directory + filename,
			std::ios::binary );
		TinyPngOut pngout( width, height, out );
		pngout.write( pixels, width * height );
		out.close();
	}





}



