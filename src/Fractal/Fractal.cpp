#include "pch.h"
#include "Fractal.h"
#include <stdio.h>

#include <Utility/Image/Image.h>
#include <Utility/Math/Vector3.h>

#include <thread>
#include <vector>


void ComputeRow(
	Image::Image* outImage,
	Vector3f input,
	float incrementX,
	uint32_t numColumns,
	uint32_t currentRow,
	const FractalFunctor3D* fractalFunctor )
{
	for( uint32_t x = 0; x < numColumns; ++x )
	{
		uint8_t r, g, b;
		fractalFunctor->GenerateColourForInput( 
			input, r, g, b );
		outImage->WritePixel( x, currentRow, r, g, b );
		input.x += incrementX;
	}
}

void FractalGenerator3D::Generate(
	Image::Image& outImage,
	const GenerateParams& params,
	const FractalFunctor3D& fractalFunctor)
{
	const Vector3f& origin = params.Origin;
	const Vector3f& scale = params.scale;
	const uint32_t w = outImage.GetWidth();
	const uint32_t h = outImage.GetHeight();

	const Vector3f bottomLeft( origin.x - ( 0.5f * scale.x ), origin.y - ( 0.5f * scale.y ), origin.z );
	const Vector3f topRight( origin.x + ( 0.5f * scale.x ), origin.y + 0.5f * scale.y, origin.z );
	const float incrementX = ( topRight.x - bottomLeft.x ) / static_cast<float>( w );
	const float incrementY = ( topRight.y - bottomLeft.y ) / static_cast<float>( h );

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
			&outImage, input, incrementX, w, y, &fractalFunctor );
		input.y += incrementY;
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}
