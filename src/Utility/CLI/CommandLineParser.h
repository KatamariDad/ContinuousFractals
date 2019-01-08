#pragma once
#include <functional>
#include <sstream>

namespace CLI
{
	// Utility function for quick and dirty command line parsing
	// TODO: Real parameter handling
	static bool Match( const char* op, int argc, char* argv[], std::function<void( char* )> action )
	{
		for (int i = 0; i < argc; ++i)
		{
			if (strcmp( op, argv[i] ) == 0)
			{
				if (i + 1 <= argc)
				{
					action( argv[i + 1] );
					return true;
				}
			}
		}
		return false;
	}

	static char** SplitParams( char* params )
	{

	}

}