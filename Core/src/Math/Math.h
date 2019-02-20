#pragma once

template<typename T>
T clamp( T f, T min, T max )
{
	f = f < min ? min : f;
	f = f > max ? max : f;
	return f;
}

template<typename T>
T max( T a, T b )
{
	return a > b ? a : b;
}

template<typename T>
T min( T a, T b )
{
	return a < b ? a : b;
}
