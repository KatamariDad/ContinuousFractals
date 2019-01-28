#pragma once
#include <math.h>
#include <limits>
#include <ostream>
#include <cassert>


class ComplexNumber
{
public:
	ComplexNumber(
		float _real,
		float _imaginary )
		: r( _real )
		, i( _imaginary )
	{}

	ComplexNumber()
		: r( 0.f )
		, i( 0.f )
	{}

	ComplexNumber( const ComplexNumber& other )
		: r( other.r )
		, i( other.i )
	{}

	bool IsInfinity() const
	{
		const float inf = std::numeric_limits<float>::infinity();
		return r == inf || r == -inf 
			|| i == inf || i == -inf;
	}

	float SquaredLength() const
	{
		return ( r * r + i * i );
	}

	float Length() const
	{
		return sqrtf( r * r + i * i );
	}

	ComplexNumber operator+( const ComplexNumber& other ) const
	{
		return ComplexNumber(
			r + other.r, 
			i + other.i);
	}

	ComplexNumber operator-( const ComplexNumber& other ) const
	{
		return ComplexNumber(
			r - other.r,
			i - other.i );
	}

	ComplexNumber operator*( const ComplexNumber& other ) const
	{
		return ComplexNumber(
			r * other.r - other.i * i,
			r * other.i + other.r * i );
	}

	bool operator==( const ComplexNumber& other ) const
	{
		return r == other.r && i == other.i;
	}

	bool operator!=( const ComplexNumber& other ) const
	{
		return r != other.r || i != other.i;
	}

	std::ostream& operator<<(std::ostream& out)
	{
		out << "(" << r << "+" << i << "i" << ")";
		return out;
	}

	static ComplexNumber WholePower( const ComplexNumber& c, uint32_t n )
	{
		assert( n > 0 );
		ComplexNumber result = c;
		for( uint32_t i = 1; i < n; ++i )
		{
			result = result * result;
		}
		return result;
	}

	float r;
	float i;

};

static ComplexNumber operator*( const float s, const ComplexNumber& v ) 
{
	return ComplexNumber(
		v.r * s,
		v.i * s );
}

static ComplexNumber operator*( const ComplexNumber& v, const float s ) 
{
	return ComplexNumber(
		v.r * s,
		v.i * s );
}

static ComplexNumber operator+( const float s, const ComplexNumber& v ) 
{
	return ComplexNumber(
		s + v.r,
		s + v.i );
}

static ComplexNumber operator+( const ComplexNumber& v, const float s ) 
{
	return ComplexNumber(
		v.r + s,
		v.i + s );
}

static ComplexNumber operator-( const float s, const ComplexNumber& v ) 
{
	return ComplexNumber(
		s - v.r,
		s - v.i );
}

static ComplexNumber operator-( const ComplexNumber& v, const float s ) 
{
	return ComplexNumber(
		v.r - s,
		v.i - s );
}

static ComplexNumber operator/( const ComplexNumber& v, const float s )
{
	return ComplexNumber(
		v.r / s,
		v.i / s );
}
