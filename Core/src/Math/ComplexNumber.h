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

	// http://mathworld.wolfram.com/ComplexDivision.html
	// The division of two complex numbers can be accomplished 
	// by multiplying the numerator and denominator by the complex 
	// conjugate of the denominator.
	ComplexNumber operator/( const ComplexNumber& other ) const
	{
		const float denominator = other.r * other.r + other.i * other.i;
		return ComplexNumber(
			( r * other.r + i * other.i ) / denominator,
			( i * other.r - r * other.i ) / denominator );
	}

	bool operator==( const ComplexNumber& other ) const
	{
		return r == other.r && i == other.i;
	}

	bool operator!=( const ComplexNumber& other ) const
	{
		return r != other.r || i != other.i;
	}

	friend std::ostream& operator<< (std::ostream& out, const ComplexNumber& c) 
	{
		out << "(" << c.r << "+" << c.i << "i" << ")";
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

static bool operator<=( const ComplexNumber& lhs, const ComplexNumber& rhs ) 
{
	return lhs.r <= rhs.r && lhs.i <= rhs.i;
}

static bool operator>=( const ComplexNumber& lhs, const ComplexNumber& rhs ) 
{
	return lhs.r >= rhs.r && lhs.i >= rhs.i;
}

static bool operator<( const ComplexNumber& lhs, const ComplexNumber& rhs ) 
{
	return lhs.r < rhs.r && lhs.i < rhs.i;
}

static bool operator>( const ComplexNumber& lhs, const ComplexNumber& rhs ) 
{
	return lhs.r > rhs.r && lhs.i > rhs.i;
}

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
