#pragma once
#include <cmath>
#include <functional>


// https://www.youtube.com/watch?v=mr5xkf6zSzk
namespace Interpolation
{
	using TransformFunc1D = std::function<float( float )>;

	inline void Clamp( float& t )
	{
		if (t > 1.f || t < -1.f)
		{
			float intPart;
			t = std::modf( t, &intPart );
		}
	}

	// TODO
	inline void BounceClamp( float& t )
	{
		t = fabs( t );
	}

	inline float Flip( float t )
	{
		Clamp( t );
		return 1 - t;
	}

	inline float SmoothStart2( float t )
	{
		Clamp( t );
		return t * t;
	}

	inline float SmoothStart3( float t )
	{
		Clamp( t );
		return t * t * t;
	}

	inline float SmoothStart4( float t )
	{
		Clamp( t );
		return t * t * t * t;
	}

	inline float SmoothStart5( float t )
	{
		Clamp( t );
		return t * t * t * t * t;
	}

	inline float SmoothStop2( float t )
	{
		t = Flip( t );
		return 1 - (t * t);
	}

	inline float SmoothStop3( float t )
	{
		t = Flip( t );
		return 1 - (t * t * t);
	}

	inline float SmoothStop4( float t )
	{
		t = Flip( t );
		return 1 - (t * t * t * t);
	}

	inline float SmoothStop5( float t )
	{
		t = Flip( t );
		return 1 - (t * t * t * t * t);
	}

	inline float FakePow( float x, float pow )
	{
		float intPart;
		pow = std::modf( pow, &intPart );
		float xMin = x, xMax = x;

		for (int i = 0; i <= (int)intPart; ++i)
		{
			if (i < intPart)
			{
				xMin *= xMin;
			}

			xMax *= xMax;
		}
		return (pow * xMax) + (1 - pow * xMin);
	}

	template<typename T>
	inline T Mix( T a, T b, float blend )
	{
		Clamp( blend );
		return ((1 - blend) * a) + (blend * b);
	}


	inline float Mix( TransformFunc1D fn1, TransformFunc1D fn2, float blend, float t )
	{
		Clamp( blend );
		return ((1 - blend) * fn1( t )) + (blend * fn2( t ));
	}

	inline float Crossfade( TransformFunc1D fn1, TransformFunc1D fn2, float t )
	{
		return Mix( fn1, fn2, t, t );
	}

	inline float Scale( TransformFunc1D fn, float t )
	{
		Clamp( t );
		return t * fn(t);
	}

	inline float ReverseScale( TransformFunc1D fn, float t )
	{
		Clamp( t );
		return (1 - t) * fn( t );
	}

	inline float Arch2( float t )
	{
		return Scale( Flip, t );
	}

	inline float SmoothStartArch3( float t )
	{
		return Scale( Arch2, t );
	}

	inline float SmoothStopArch3( float t )
	{
		return ReverseScale( Arch2, t );
	}

	inline float SmoothStepArch4( float t )
	{
		TransformFunc1D arch2Scaled = []( float t ) 
		{
			return Scale( Arch2, t );
		};
		return ReverseScale( arch2Scaled, t );
	}

	inline float BellCurve6( float t )
	{
		return SmoothStop3( t ) * SmoothStart3( t );
	}

	class InterpolationTestRunner
	{
	public:
		InterpolationTestRunner() = delete;
		InterpolationTestRunner( const char* dir ) : m_directory( dir ) {}

		void Run();
	private:
		const char* m_directory;
	};
}
