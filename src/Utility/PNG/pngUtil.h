#pragma once

#include <cstdint>
#include <string>

namespace PNG
{
	void SavePNG(
		const std::string& directory,
		const std::string& filename,
		const uint8_t* pixels,
		uint32_t width,
		uint32_t height );


}



