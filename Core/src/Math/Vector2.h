#pragma once
#include <math.h>
#include <limits>
#include <ostream>
#include <cassert>


class Vector2f
{
public:
	Vector2f()
		: x(0.f)
		, y(0.f)
	{
	}

	Vector2f(
		float _x, 
		float _y)
		: x(_x)
		, y(_y)
	{
	}

	Vector2f(
		float _c )
		: x(_c)
		, y(_c)
	{
	}

	Vector2f( const Vector2f& other )
		: x( other.x )
		, y( other.y )
	{}

	bool IsZero() const
	{
		return x == 0.f && y == 0.f;
	}

	bool IsInfinity() const
	{
		const float inf = std::numeric_limits<float>::infinity();
		return x == inf || y == inf 
			|| x == -inf || y == -inf ;
	}

	float operator[]( size_t i ) const
	{
		switch( i )
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			assert( false );
		}
		return 0.f;
	}

	Vector2f operator+( const Vector2f& other ) const
	{
		return Vector2f(
			x + other.x,
			y + other.y );
	}

	Vector2f operator-( const Vector2f& other ) const
	{
		return Vector2f(
			x - other.x,
			y - other.y );
	}

	Vector2f operator/( const float s ) const
	{
		assert( s != 0.f );
		return Vector2f(
			x / s,
			y / s );
	}

	float SquaredLength() const
	{
		return ( x * x + y * y );
	}

	float Length() const
	{
		return sqrtf( x * x + y * y );
	}

	Vector2f ComputeNormal() const
	{
		return *this / Length();
	}

	void Normalize() 
	{
		const float length = Length();
		assert( length > 0.f );
		x /= length;
		y /= length;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector2f& v)
	{
		out << "(" << v.x << "," << v.y << ")";
		return out;
	}

	float x;
	float y;
};

static Vector2f operator*( const float s, const Vector2f& v ) 
{
	return Vector2f(
		v.x * s,
		v.y * s );
}

static Vector2f operator*( const Vector2f& v, const float s ) 
{
	return Vector2f(
		v.x * s,
		v.y * s );
}

static Vector2f operator+( const float s, const Vector2f& v ) 
{
	return Vector2f(
		s + v.x,
		s + v.y );
}

static Vector2f operator+( const Vector2f& v, const float s ) 
{
	return Vector2f(
		v.x + s,
		v.y + s );
}

static Vector2f operator-( const float s, const Vector2f& v ) 
{
	return Vector2f(
		s - v.x,
		s - v.y );
}

static Vector2f operator-( const Vector2f& v, const float s ) 
{
	return Vector2f(
		v.x - s,
		v.y - s );
}


static Vector2f operator/( const float s, const Vector2f& v )
{
	assert( v.x != 0.f && v.y != 0.f );
	return Vector2f(
		s / v.x,
		s / v.y );
}

static bool operator<( const Vector2f& a, const Vector2f& b )
{
	return a.x < b.x && a.y < b.y;
}

static bool operator>( const Vector2f& a, const Vector2f& b )
{
	return a.x > b.x && a.y > b.y;
}

static float DotProduct( const Vector2f& a, const Vector2f& b )
{
	return
		a.x * b.x +
		a.y * b.y;

}
