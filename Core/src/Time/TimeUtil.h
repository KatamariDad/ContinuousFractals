#pragma once
#include <string>
#include <chrono>

namespace Time
{
	std::string CurrentTimeAndDate()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t( now );

		time_t t = time( NULL );
		struct tm buf;
		::localtime_s( &buf, &t );
		char str[26];
		::asctime_s( str, sizeof str, &buf );
		for (char& c : str)
		{
			if (c == ':')
			{
				c = '-';
			}
		}
		std::string ret( str );
		return ret.substr( 0, ret.size() - 1 );
	}
}