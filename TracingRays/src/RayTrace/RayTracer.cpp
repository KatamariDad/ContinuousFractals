#include "pch.h"
#include "RayTracer.h"

#include <iostream>

#include <Image/Image.h>
#include <Image/PixelColour.h>
#include <Scene/Material/Material.h>
#include <Scene/SceneNode.h>

#define PI 3.14159265358979323846
#define FIXED_IMAGE_SIZE 256
#define NUM_THREADS 16

void RayTracer::Trace(const TraceParameters* inputParameters, Image::Image* image) 
{
	const TraceParameters& params = *inputParameters;
	Image::Image& outImage = *image;
    std::cout<<"Thread "<<params.m_threadId<<" start"<<std::endl;
    constexpr size_t fixedImageSize = FIXED_IMAGE_SIZE;

	PreLightingParams preLightingParams( 
		params.m_sceneRoot, 
		params.m_eye, 
		Vector3f(), 
		params.m_ambient, 
		params.m_lights );

	for (size_t y = params.m_startY; y < params.m_endY; ++y) 
    {
        PixelColour pixelColour[fixedImageSize];
		for (size_t x = params.m_startX; x < params.m_endX; ++x) 
        {
            const Vector3f pixelWorldLocation( 
                params.m_topLeft.x + (double)x * params.m_deltaX,
                params.m_topLeft.y - (double)y * params.m_deltaY,
                params.m_projectionDistance);
            preLightingParams.m_rayDir = ( pixelWorldLocation - params.m_eye ).ComputeNormal();

                
			PixelColour colour;
			const bool hitPrimitive = RayTracer::TraceInternal( preLightingParams, colour );

            if (!hitPrimitive)
            {
				colour = Colours::WHITE;
                //RayTracer::Generate_Background( 
                //    fixedImageSize, fixedImageSize, x, y, 
                //    pixelColour[x]);
            }

			outImage.WritePixel( x, y, colour );

		}
	}
    std::cout<<"Thread "<<params.m_threadId<<" end"<<std::endl;
}


bool RayTracer::TraceInternal(
	const PreLightingParams& params,
	PixelColour& outPixelColour )
{
    Vector3f hitPoint, hitNormal;
    const Material* hitMaterial = nullptr;
    const bool doesIntersect = params.m_sceneRoot->IntersectRay(
        params.m_rayOrigin,
        params.m_rayDir,
        hitPoint,
        hitNormal,
        hitMaterial );

    if (!doesIntersect)
    {
        return false;
    }

	hitMaterial->ApplyMaterial( 
		params.m_rayDir, 
		hitPoint, hitNormal, 
		params.m_lights, 
		outPixelColour );

    return true;
}
