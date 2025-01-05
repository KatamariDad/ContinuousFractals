#pragma once

#define NOMINMAX

#include <string>
#include <Windows.h>


namespace FileSystem
{
	static bool MakeDirectory( const std::string& directory )
	{
		return CreateDirectoryA( directory.c_str(), NULL );
	}
}