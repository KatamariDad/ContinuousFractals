#include "pch.h"
#include "Fractal.h"
#include <stdio.h>

#include <Fractal/Colourizers/Colourizers.h>
#include <Image/Image.h>
#include <Math/Vector3.h>

#include <thread>
#include <vector>


void ComputeRow(
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
		outImage->WritePixel( x, currentRow, r, g, b );
		input.x += incrementX;
	}
}

void FractalGenerator3D::Generate(
	Image::Image& outImage,
	const GenerateParams& params,
	const FractalFunctor3D& fractalFunctor)
{
	const Vector3f& origin = params.origin;
	const Vector3f& scale = params.scale;
	const uint32_t w = outImage.GetWidth();
	const uint32_t h = outImage.GetHeight();

	const Vector3f bottomLeft( origin.x - ( 0.5f * scale.x ), origin.y - ( 0.5f * scale.y ), origin.z );
	const Vector3f topRight( origin.x + ( 0.5f * scale.x ), origin.y + 0.5f * scale.y, origin.z );
	const float incrementX = ( topRight.x - bottomLeft.x ) / static_cast<float>( w );
	const float incrementY = ( topRight.y - bottomLeft.y ) / static_cast<float>( h );

	if( params.multithreadEnabled )
	{
		std::vector<std::thread> threads;
		for( uint32_t y = 0; y < h; ++y )
		{
			threads.push_back( std::thread() );
		
		}
		Vector3f input( bottomLeft );
		for( uint32_t y = 0; y < h; ++y )
		{
			input.x = bottomLeft.x;
			threads[y] = std::thread( 
				ComputeRow, 
				&outImage, 
				input, 
				incrementX, 
				w, y, 
				&params.colourizer,
				&fractalFunctor );
			input.y += incrementY;
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}
	else
	{
		Vector3f input( bottomLeft );
		for( uint32_t y = 0; y < h; ++y )
		{
			input.x = bottomLeft.x;
			ComputeRow( 
				&outImage, 
				input, 
				incrementX, 
				w, y, 
				&params.colourizer, 
				&fractalFunctor );
			input.y += incrementY;
		}
	}
}


FractalGenerator3D::GenerateParams::GenerateParams(const FractalColourizer& _colourizer)
	: scale( 6.f )
	, origin( 0.f )
	, colourizer(_colourizer)
	, multithreadEnabled( true )
{}

FractalGenerator3D::GenerateParams::~GenerateParams()
{
}