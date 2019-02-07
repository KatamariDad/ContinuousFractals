#include "pch.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <string>

#include <Scene/SceneNode.h>
#include <Scene/Geometry/Sphere/Sphere.h>
#include <Scene/Material/Material.h>
#include <Scene/Material/PhongMaterial.h>
#include <Scene/Light/Light.h>
#include <Image/GIF/GifUtil.h>
#include <Image/Image.h>
#include <RayTrace/RayTracer.h>


#define PI 3.14159265358979323846
#define FIXED_IMAGE_SIZE 256 
#define NUM_THREADS 16


void RenderScene(
		// What to render
		SceneNode* root,
		// Image to write to, set to a given width and height
		Image::Image& image,
		// Viewing parameters
		const Vector3f& eye,
		const Vector3f& view,
		const Vector3f& up,
		double fovy,
		// Lighting parameters
		const Vector3f& ambient,
		const std::vector<const Light *>& lights
) {

    // Render the Scene
    constexpr size_t fixedImageSize = FIXED_IMAGE_SIZE;
	const size_t h = fixedImageSize; //image.height();
	const size_t w = fixedImageSize; //image.width();
    const float r = h / w;
    const float d = 1.f;
    const float fovy_radian = fovy * (PI / 180.f);

	const float windowH = 2 * ( d / std::tan( fovy_radian ) );
    const float windowW = r * windowH;
    const float deltaX = windowW / (float)w;
    const float deltaY = windowH / (float)h;

    const float projection_distance = d; 
    const Vector3f forward = view.ComputeNormal();
    const Vector3f projection_centerpoint = eye + projection_distance * forward;
    const Vector3f left = CrossProduct(forward, up.ComputeNormal());

    const Vector3f topLeft = 
        projection_centerpoint + ((double)windowW / 2.f) * left + ((double)windowH / 2.f) * up;

    RayTracer::TraceParameters params[NUM_THREADS];
    std::thread threads[NUM_THREADS];
    
    size_t width_x = fixedImageSize; // (float)fixedImageSize / (float)NUM_THREADS;
    float width_y = (float)fixedImageSize / (float)NUM_THREADS;
    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        params[i] = RayTracer::TraceParameters(
            topLeft, projection_distance, deltaX, deltaY,
            root, eye, view, up, ambient, lights, i
            );
        params[i].SetIndices(
            0, fixedImageSize,
            width_y * i, (width_y * (i + 1)));

        threads[i] =  std::thread(RayTracer::Trace, &params[i], &image);
    }

    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

}


int main()
{
	// lights
	Light topRightLight( 
		Vector3f( 1.f, 3.f, -2.5f ), 
		Vector3f( 20, 0, 0 ) );
	Light bottomLight( 
		Vector3f( 0.f, -3.f, -2.5f ), 
		Vector3f( 200, 200, 200 ) );

	std::vector<const Light*> lights;
	lights.push_back( &bottomLight );
	lights.push_back( &topRightLight );

	PhongMaterial phongMaterial(
		Vector3f( 5.f, 1.f, 0.f ),
		Vector3f( 1.f, 0.f, 0.f ),
		Vector3f( 0.f, 0.f, 30.f ),
		100, 100 );

	Sphere sphere( 2.f );
	// look down the Z positive axis
	const Vector3f eyePosition( 0.f, 0.f, -3.f );
	const Vector3f view( 0.f, 0.f, 1.f );
	const Vector3f up( 0.f, 1.f, 0.f );
	const Vector3f ambientLight( 0.f, 0.f, 0.f ); // TODO: as PixelColour
	double fovy = 15.f;

	const std::string directory( "D:\\Projects\\ContinuousFractals\\out\\" );
	const std::string gifName = directory + "rayTrace.gif";

	Image::Gif giraffe( gifName.c_str(), FIXED_IMAGE_SIZE, FIXED_IMAGE_SIZE );
	Vector3f sceneLocation( -3.f, 0.f, 0.f );
	for( size_t i = 0; i < 70; ++i )
	{
		sceneLocation.x += 0.1f;
		sceneLocation.y = std::sinf( sceneLocation.x  * 2);
		//if( i % 2 == 0 )
		{
			//phongMaterial.m_diffuse.z = i * 0.1f;
			//phongMaterial.m_diffuse.x = 5.f - ( i * 0.1f );
		}
		//else
		//{
		//	phongMaterial.m_diffuse.z = 0.f;
		//	phongMaterial.m_diffuse.x = 0.f;
		//}

		SceneNode root( sceneLocation, sphere, phongMaterial );
		std::stringstream filename;
		filename << "rayTrace_" << i << ".png";
		Image::Image image( FIXED_IMAGE_SIZE, FIXED_IMAGE_SIZE, directory + filename.str() );

		RenderScene( &root,
			image, eyePosition,
			view, up, fovy, 
			ambientLight, lights );
	
		giraffe.AddFrame( image );
		image.Save();
	}
	giraffe.Save();


	return 0;
}


