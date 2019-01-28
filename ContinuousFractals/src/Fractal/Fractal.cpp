#include "pch.h"
#include "Fractal.h"
#include <stdio.h>

#include <Fractal/Colourizers/Colourizers.h>
#include <Image/Image.h>
#include <Math/Vector3.h>

#include <thread>
#include <vector>


void ComputeRowFractalFunctor3D(
	Image::Image* outImage,
	Vector3f input,
	float incrementX,
	uint32_t numColumns,
	uint32_t currentRow,
	const FractalColourizer* colourizer,
	const FractalFunctor3D* fractalFunctor )
{
	for( uint32_t x = 0; x < numColumns; ++x )
	{
		PixelColour p;
		fractalFunctor->GenerateColourForInput( 
			input, 
			*colourizer,
			p );
		outImage->WritePixel( x, currentRow, p );
		input.x += incrementX;
	}
}

void ComputeRowFractalFunctor2DComplex(
	Image::Image* outImage,
	ComplexNumber input,
	float incrementX,
	uint32_t numColumns,
	uint32_t currentRow,
	const FractalColourizer* colourizer,
	const FractalFunctor2DComplex* fractalFunctor )
{
	for( uint32_t x = 0; x < numColumns; ++x )
	{
		PixelColour p;
		fractalFunctor->GenerateColourForInput( 
			input, 
			*colourizer,
			p );
		outImage->WritePixel( x, currentRow, p );
		input.r += incrementX;
	}
}

void FractalGenerator::Generate(
	Image::Image& outImage,
	const GenerateParams& params,
	const FractalFunctor3D& fractalFunctor)
{
	const Vector3f& origin = params.origin;
	const Vector3f& scale = params.scale;
	const uint32_t w = outImage.GetWidth();
	const uint32_t h = outImage.GetHeight();

	const Vector3f topLeft( origin.x - ( 0.5f * scale.x ), origin.y + ( 0.5f * scale.y ), origin.z );
	const Vector3f bottomRight( origin.x + ( 0.5f * scale.x ), origin.y - 0.5f * scale.y, origin.z );
	const float incrementX = ( bottomRight.x - topLeft.x ) / static_cast<float>( w );
	const float incrementY = ( topLeft.y - bottomRight.y ) / static_cast<float>( h );

	if( params.multithreadEnabled )
	{
		std::vector<std::thread> threads;
		for( uint32_t y = 0; y < h; ++y )
		{
			threads.push_back( std::thread() );
		
		}
		Vector3f input( topLeft );
		for( uint32_t y = 0; y < h; ++y )
		{
			input.x = topLeft.x;
			threads[y] = std::thread( 
				ComputeRowFractalFunctor3D, 
				&outImage, 
				input, 
				incrementX, 
				w, y, 
				&params.colourizer,
				&fractalFunctor );
			input.y -= incrementY;
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}
	else
	{
		Vector3f input( topLeft );
		for( uint32_t y = 0; y < h; ++y )
		{
			input.x = topLeft.x;
			ComputeRowFractalFunctor3D( 
				&outImage, 
				input, 
				incrementX, 
				w, y, 
				&params.colourizer, 
				&fractalFunctor );
			input.y -= incrementY;
		}
	}
}


void FractalGenerator::Generate(
	Image::Image& outImage,
	const GenerateParams& params,
	const FractalFunctor2DComplex& fractalFunctor )
{
	const Vector3f& origin = params.origin;
	const Vector3f& scale = params.scale;
	const uint32_t w = outImage.GetWidth();
	const uint32_t h = outImage.GetHeight();

	const ComplexNumber topLeft( origin.x - ( 0.5f * scale.x ), origin.y + ( 0.5f * scale.y ) );
	const ComplexNumber bottomRight( origin.x + ( 0.5f * scale.x ), origin.y - 0.5f * scale.y );
	const float incrementX = ( bottomRight.r - topLeft.r ) / static_cast<float>( w );
	const float incrementY = ( topLeft.i - bottomRight.i ) / static_cast<float>( h );;

	if( params.multithreadEnabled )
	{
		std::vector<std::thread> threads;
		for( uint32_t y = 0; y < h; ++y )
		{
			threads.push_back( std::thread() );
		
		}
		ComplexNumber input( topLeft );
		for( uint32_t y = 0; y < h; ++y )
		{
			input.r = topLeft.r;
			threads[y] = std::thread( 
				ComputeRowFractalFunctor2DComplex, 
				&outImage, 
				input, 
				incrementX, 
				w, y, 
				&params.colourizer,
				&fractalFunctor );
			input.i -= incrementY;
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}
	else
	{
		ComplexNumber input( topLeft );
		for( uint32_t y = 0; y < h; ++y )
		{
			input.r = topLeft.r;
			ComputeRowFractalFunctor2DComplex( 
				&outImage, 
				input, 
				incrementX, 
				w, y, 
				&params.colourizer, 
				&fractalFunctor );
			input.i -= incrementY;
		}
	}
}

FractalGenerator::GenerateParams::GenerateParams(const FractalColourizer& _colourizer)
	: scale( 6.f )
	, origin( 0.f )
	, colourizer(_colourizer)
	, multithreadEnabled( true )
{}

FractalGenerator::GenerateParams::~GenerateParams()
{
}