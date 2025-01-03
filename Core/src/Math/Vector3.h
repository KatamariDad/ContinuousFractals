#pragma once
#include <math.h>
#include <limits>
#include <ostream>
#include <cassert>


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

	bool IsZero() const
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	bool IsInfinity() const
	{
		constexpr float inf = std::numeric_limits<float>::infinity();
		return x == inf || y == inf || z == inf
			|| x == -inf || y == -inf || z == -inf;
	}

	float operator[]( size_t i ) const
	{
		switch( i )
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			assert( false );
		}
		return 0.f;
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

	Vector3f operator/( const float s ) const
	{
		assert( s != 0.f );
		return Vector3f(
			x / s,
			y / s,
			z / s );
	}

	float SquaredLength() const
	{
		return ( x * x + y * y + z * z );
	}

	float Length() const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	Vector3f ComputeNormal() const
	{
		return *this / Length();
	}

	void Normalize() 
	{
		const float length = Length();
		assert( length > 0.f );
		x /= length;
		y /= length;
		z /= length;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector3f& v)
	{
		out << "(" << v.x << "," << v.y << "," << v.z << ")";
		return out;
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

static Vector3f operator*( const Vector3f& v1, const Vector3f& v2 ) 
{
	return Vector3f(
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z );
}

static Vector3f operator/( const float s, const Vector3f& v )
{
	assert( v.x != 0.f && v.y != 0.f && v.z != 0.f );
	return Vector3f(
		s / v.x,
		s / v.y,
		s / v.z);
}

static float DotProduct( const Vector3f& a, const Vector3f& b )
{
	return
		a.x * b.x +
		a.y * b.y +
		a.z * b.z;

}

static Vector3f CrossProduct( const Vector3f& a, const Vector3f& b )
{
	return Vector3f(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x );
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
		constexpr double inf = std::numeric_limits<double>::infinity();
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

