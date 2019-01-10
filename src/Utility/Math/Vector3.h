#pragma once
#include <math.h>
#include <limits>


class Vector3f
{
public:
	Vector3f()
		: x(0.f)
		, y(0.f)
		, z(0.f)
	{
	}

	Vector3f(
		float _x, 
		float _y, 
		float _z )
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}

	Vector3f(
		float _c )
		: x(_c)
		, y(_c)
		, z(_c)
	{
	}

	Vector3f( const Vector3f& other )
		: x( other.x )
		, y( other.y )
		, z( other.z )
	{}

	bool IsInfinity() const
	{
		const float inf = std::numeric_limits<float>::infinity();
		return x == inf || y == inf || z == inf
			|| x == -inf || y == -inf || z == -inf;
	}

	Vector3f operator+( const Vector3f& other ) const
	{
		return Vector3f(
			x + other.x, 
			y + other.y, 
			z + other.z);
	}

	Vector3f operator-( const Vector3f& other ) const
	{
		return Vector3f(
			x - other.x,
			y - other.y,
			z - other.z );
	}

	Vector3f operator*( const float s ) const
	{
		return Vector3f(
			x * s,
			y * s,
			z * s );
	}

	float SquaredLength() const
	{
		return ( x * x + y * y + z * z );
	}

	float Length() const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float x;
	float y;
	float z;
};

static Vector3f operator*( const float s, const Vector3f& v ) 
{
	return Vector3f(
		v.x * s,
		v.y * s,
		v.z * s );
}

static Vector3f operator*( const Vector3f& v, const float s ) 
{
	return Vector3f(
		v.x * s,
		v.y * s,
		v.z * s );
}

static Vector3f operator+( const float s, const Vector3f& v ) 
{
	return Vector3f(
		s + v.x,
		s + v.y,
		s + v.z );
}

static Vector3f operator+( const Vector3f& v, const float s ) 
{
	return Vector3f(
		v.x + s,
		v.y + s,
		v.z + s );
}

static Vector3f operator-( const float s, const Vector3f& v ) 
{
	return Vector3f(
		s - v.x,
		s - v.y,
		s - v.z );
}

static Vector3f operator-( const Vector3f& v, const float s ) 
{
	return Vector3f(
		v.x - s,
		v.y - s,
		v.z - s );
}


static Vector3f operator/( const Vector3f& v, const float s ) 
{
	return Vector3f(
		v.x / s,
		v.y / s,
		v.z / s );
}



class Vector3d
{
public:
	Vector3d()
		: x(0.f)
		, y(0.f)
		, z(0.f)
	{
	}

	Vector3d(
		double _x, 
		double _y, 
		double _z )
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}

	Vector3d(
		double _c )
		: x(_c)
		, y(_c)
		, z(_c)
	{
	}

	Vector3d( const Vector3d& other )
		: x( other.x )
		, y( other.y )
		, z( other.z )
	{}

	bool IsInfinity() const
	{
		const double inf = std::numeric_limits<double>::infinity();
		return x == inf || y == inf || z == inf
			|| x == -inf || y == -inf || z == -inf;
	}

	Vector3d operator+( const Vector3d& other ) const
	{
		return Vector3d(
			x + other.x, 
			y + other.y, 
			z + other.z);
	}

	Vector3d operator-( const Vector3d& other ) const
	{
		return Vector3d(
			x - other.x,
			y - other.y,
			z - other.z );
	}

	Vector3d operator*( const double s ) const
	{
		return Vector3d(
			x * s,
			y * s,
			z * s );
	}

	double SquaredLength() const
	{
		return ( x * x + y * y + z * z );
	}

	double Length() const
	{
		return sqrt( x * x + y * y + z * z );
	}

	double x;
	double y;
	double z;
};

static Vector3d operator*( const double s, const Vector3d& v ) 
{
	return Vector3d(
		v.x * s,
		v.y * s,
		v.z * s );
}

static Vector3d operator*( const Vector3d& v, const double s ) 
{
	return Vector3d(
		v.x * s,
		v.y * s,
		v.z * s );
}

static Vector3d operator+( const double s, const Vector3d& v ) 
{
	return Vector3d(
		s + v.x,
		s + v.y,
		s + v.z );
}

static Vector3d operator+( const Vector3d& v, const double s ) 
{
	return Vector3d(
		v.x + s,
		v.y + s,
		v.z + s );
}

static Vector3d operator-( const double s, const Vector3d& v ) 
{
	return Vector3d(
		s - v.x,
		s - v.y,
		s - v.z );
}

static Vector3d operator-( const Vector3d& v, const double s ) 
{
	return Vector3d(
		v.x - s,
		v.y - s,
		v.z - s );
}


static Vector3d operator/( const Vector3d& v, const double s ) 
{
	return Vector3d(
		v.x / s,
		v.y / s,
		v.z / s );
}

